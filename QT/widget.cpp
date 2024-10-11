#include "widget.hpp"
#include "ui_widget.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTime>

#define RE35_REDUCTION_RATIO 35 // 减速比
#define ENCODER_LINES_NUM 2000  // 编码器线数
#define PWM_LIM 5000            // pwm限制值
#define LEAD 5                  // 丝杠导程（mm）

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);

  // 枚举所有可用的串口
  QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
  if (ports.isEmpty()) {
    qDebug() << "No serial ports found.";
  }
  // 遍历所有串口
  foreach (const QSerialPortInfo &info, ports) {
    qDebug() << "Found serial port:" << info.portName()
             << ", description:" << info.description();
    ui->port_name->addItem(info.portName());
  }
}

Widget::~Widget() { delete ui; }

void Widget::on_openSerialPort_clicked() {
  if (ui->openSerialPort->text() == QString("打开")) {
    if (!manager_.open(ui->port_name->currentText(),
                       static_cast<QSerialPort::BaudRate>(
                           ui->baudrate->currentText().toInt()))) {
      return;
    }

    qDebug() << "Open port " << ui->port_name->currentText();
    // 成功打开串口，改变按钮上的文本
    ui->openSerialPort->setText("关闭");
  } else {
    ui->openSerialPort->setText("打开");
    manager_.close();
  }
}

// 将结构体转换为字符数组
void Widget::CmdToCharArray(const Cmd &cmd, char *array) {
  // 复制header
  for (int i = 0; i < 6; ++i) {
    array[i] = cmd.header[i];
  }
  // 复制指令数组，注意字节序
  for (int i = 0; i < 3; ++i) {
    memcpy(&array[6 + i * sizeof(short)], &cmd.vel[i], sizeof(short));
  }
  for (int i = 0; i < 3; ++i) {
    memcpy(&array[12 + i * sizeof(float)], &cmd.pos[i], sizeof(float));
  }
  // 复制tailer
  array[24] = cmd.tailer;
}

void Widget::on_startJog_clicked() {
  std::vector<float> origin = {0, 0, 0};
  cmd_.pos[0] = (float)ui->XPos->value() - origin[0];
  cmd_.pos[1] = (float)ui->YPos->value() - origin[1];
  cmd_.pos[2] = (float)ui->ZPos->value() - origin[2];
  cmd_.vel[0] = (short)ui->Jog_Vel->value();
  cmd_.vel[1] = (short)ui->Jog_Vel->value();
  cmd_.vel[2] = (short)ui->Jog_Vel->value();

  char dataArray[sizeof(Cmd)];
  CmdToCharArray(cmd_, dataArray);
  // for (int i = 0; i < sizeof(Cmd); ++i)
  //   printf("%02X ", static_cast<unsigned char>(dataArray[i]));
  // std::cout << std::endl;
  manager_.sendCmd(dataArray, sizeof(dataArray));
}

void Widget::on_stopJog_clicked() {
  cmd_.vel[0] = 0;
  cmd_.vel[1] = 0;
  cmd_.vel[2] = 0;

  char dataArray[sizeof(Cmd)];
  CmdToCharArray(cmd_, dataArray);
  manager_.sendCmd(dataArray, sizeof(dataArray));
}

/*Traj
 * Button*****************************************************************************************************************************/

void Widget::on_readTraj_clicked() {
  QString path{};
  if (0 == ui->traj->currentIndex()) // Robot_CoM traj
    path = QCoreApplication::applicationDirPath() + "/../traj/Robot_CoM.txt";
  qDebug() << path;

  ReadTraj(path, traj_);
}

void Widget::on_startTraj_clicked() {
  qInfo("Start running traj");

  QString fileName =
      QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_send.csv";
  QFile csvFile;
  csvFile.setFileName(fileName);
  if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text |
                    QIODevice::Append)) {
    qWarning() << "Failed to open CSV file:" << csvFile.errorString();
    return;
  }
  QTextStream out(&csvFile);

  QDateTime currentTime;
  qint64 currentTimeMSecsSinceEpoch;

  float period = ui->trajPeriod->value();
  float last_cmd_pos[3] = {0};
  char dataArray[sizeof(Cmd)];

  for (int i = 1; i < traj_.size(); ++i) {
    if (stop_traj_)
      break;

    for (int j = 0; j < 3; ++j) {
      cmd_.pos[j] = traj_[i][j] - traj_[0][j];
      cmd_.vel[j] = round(abs(cmd_.pos[j] - last_cmd_pos[j]) * 60 *
                          RE35_REDUCTION_RATIO / (period * LEAD));
      last_cmd_pos[j] = cmd_.pos[j];
    }

    CmdToCharArray(cmd_, dataArray);
    manager_.sendCmd(dataArray, sizeof(dataArray));

    // qDebug() << cmd_.vel[0] << cmd_.vel[1] << cmd_.vel[2];

    currentTime = QDateTime::currentDateTime();
    currentTimeMSecsSinceEpoch = currentTime.toMSecsSinceEpoch();
    out << currentTimeMSecsSinceEpoch << "," << cmd_.pos[0] << ","
        << cmd_.pos[1] << "," << cmd_.pos[2] << "\n";

    DelayMSec(period * 1000);
  }

  qInfo("Finish running traj");
}

void Widget::on_stopTraj_clicked() {
  qInfo("Stop running traj");
  stop_traj_ = true;
}

void Widget::ReadTraj(const QString &path, QList<QList<double>> &traj) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Could not open file:" << file.errorString();
    return;
  }

  QTextStream in(&file);
  QString line;
  while (!in.atEnd()) {
    line = in.readLine();
    QStringList parts = line.split('\t'); // 使用制表符分割

    if (parts.size() == 3) {
      QList<double> point;
      bool ok1, ok2, ok3;
      double x = parts[0].toDouble(&ok1);
      double y = parts[1].toDouble(&ok2);
      double z = parts[2].toDouble(&ok3);

      if (ok1 && ok2 && ok3) {
        point << x << y << z;
        traj.append(point);
      } else {
        qDebug() << "Error parsing line:" << line;
        return;
      }
    } else {
      qDebug() << "Invalid number of columns in line:" << line;
      return;
    }
  }

  qDebug() << "Read traj finished.";
  file.close();
}

void Widget::DelayMSec(unsigned int msec) {
  QTime _Timer = QTime::currentTime().addMSecs(msec);
  while (QTime::currentTime() < _Timer)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
