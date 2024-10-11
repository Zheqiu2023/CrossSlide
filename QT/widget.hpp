#pragma once

#include <QWidget>

#include "serial_manager.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

#pragma pack(push, 1)
struct Cmd {
  unsigned char header[6] = {0xfe, 0x10, 0x90, 0x90, 0x02, 0x00};
  short vel[3];
  float pos[3];
  unsigned char tailer = 0xff;
};
#pragma pack(pop)

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  void CmdToCharArray(const Cmd &cmd, char *array);
  void ReadTraj(const QString &path, QList<QList<double>> &traj);
  void DelayMSec(unsigned int msec);

private slots:

  void on_openSerialPort_clicked();

  void on_readTraj_clicked();

  void on_startTraj_clicked();

  void on_stopTraj_clicked();

  void on_stopJog_clicked();

  void on_startJog_clicked();

private:
  Ui::Widget *ui;
  serial_manager::SerialManager manager_;
  Cmd cmd_;

  bool stop_traj_ = false;
  QList<QList<double>> traj_{};
};
