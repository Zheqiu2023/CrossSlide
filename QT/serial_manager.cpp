#include "serial_manager.hpp"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <iostream>

using namespace serial_manager;

SerialManager::SerialManager(QObject *parent)
    : QObject(parent), serial_(new QSerialPort(this)) {
  connect(serial_, &QSerialPort::readyRead, this, &SerialManager::recvData);

  // 设置文件名
  fileName_ =
      QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_recv.csv";
  csvFile_.setFileName(fileName_);

  // 尝试打开文件
  if (!csvFile_.open(QIODevice::WriteOnly | QIODevice::Text |
                     QIODevice::Append)) {
    qWarning() << "Failed to open CSV file:" << csvFile_.errorString();
    return;
  }
}

SerialManager::~SerialManager() {
  if (serial_) {
    if (serial_->isOpen())
      serial_->close();
    delete serial_;
  }
}

bool SerialManager::open(const QString &port_name,
                         QSerialPort::BaudRate baudrate,
                         QSerialPort::DataBits dataBits,
                         QSerialPort::Parity parity,
                         QSerialPort::StopBits stopBits,
                         QSerialPort::FlowControl flowControl) {
  if (!serial_->isOpen()) {
    serial_->setPortName(port_name);      // 串口号
    serial_->setBaudRate(baudrate);       // 波特率
    serial_->setDataBits(dataBits);       // 数据位
    serial_->setParity(parity);           // 校验位
    serial_->setStopBits(stopBits);       // 停止位
    serial_->setFlowControl(flowControl); // 流控制

    if (!serial_->open(QIODevice::ReadWrite)) {
      qDebug() << "Open serial port " << port_name << " failed.";
      return false;
    }
    return true;
  } else {
    qDebug() << "Close port " << port_name << ". Please reopen it.";
    serial_->close();
    return false;
  }
}

void SerialManager::close() {
  if (serial_->isOpen()) {
    serial_->clear(); // 清空缓存区
    serial_->close();
    csvFile_.close();
    qDebug() << "Close port " << serial_->portName();
  }
}

void SerialManager::sendCmd(const char *data, qint64 size) {
  if (serial_->isOpen()) {
    if (serial_->write(data, size) == -1)
      qDebug() << "Failed to write data to serial port.";
    // qDebug() << "Sent" << bytesWritten << "bytes.";
  } else {
    qDebug() << "Serial port not open.";
  }
}

void SerialManager::recvData() {
  QByteArray rxbuff = serial_->readLine();
  // qDebug() << rxbuff;

  for (int i = 0; i < rxbuff.size(); i++) {
    Res_ = rxbuff[i];
    if ((USART_RX_STA_ & 0x8000)) // 接收已开始
    {
      if ((USART_RX_STA_ & 0x4000)) {
        USART_RX_BUF_[USART_RX_STA_ & 0X3FFF] = rxbuff[i];
        // USART_RX_BUF.append(data[i]) ;
        // qDebug()<<"接收到第数据位"<<USART_RX_STA<<USART_RX_BUF[0]<<endl;
        USART_RX_STA_++;
        if ((USART_RX_STA_ & 0X3FFF) > (USART_REC_LEN - 1)) {
          USART_RX_STA_ = 0;
          // USART_RX_BUF.clear();
        } // 接收完成
      } else {
        if (Res_ == 0x55) {
          USART_RX_STA_ = 0; // 先给后面的计数值清零
          USART_RX_STA_ |= 0x8000;
          USART_RX_STA_ |= 0x4000;
          // qDebug()<<"接收到第二位"<<USART_RX_STA_<<endl;
        } else
          USART_RX_STA_ = 0; // 接收错误,重新开始
      }
    } else {
      if (Res_ == 0xAA) {
        USART_RX_STA_ |= 0x8000;
        // qDebug()<<"接收到第一位"<<USART_RX_STA_<<endl;
      } else
        USART_RX_STA_ = 0; // 接收错误,重新开始
    }
  }

  if (USART_RX_STA_ == 0) {
    QTextStream out(&csvFile_);
    // 获取当前系统时间
    QDateTime currentTime = QDateTime::currentDateTime();
    // msecs since 1970/1/1
    qint64 currentTimeMSecsSinceEpoch = currentTime.toMSecsSinceEpoch();

    // for (int i = 0; i < USART_REC_LEN; ++i)
    //   printf("%02X ", static_cast<unsigned char>(USART_RX_BUF_[i]));
    // std::cout << std::endl;

    if (USART_RX_BUF_[18] == 0xcc && USART_RX_BUF_[19] == 0x88) {
      MotorFBData data{};
      memcpy(&data, USART_RX_BUF_, sizeof(MotorFBData));
      out << currentTimeMSecsSinceEpoch << "," << data.vel[0] << ","
          << data.pos[0] << "," << data.vel[1] << "," << data.pos[1] << ","
          << data.vel[2] << "," << data.pos[2] << "\n";
    }
  }
}
