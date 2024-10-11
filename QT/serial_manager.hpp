#pragma once

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>     // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo> // 提供系统中存在的串口信息

#define USART_REC_LEN 20 // 定义最大接收字节数

namespace serial_manager {
class SerialManager : public QObject {
  Q_OBJECT
public:
  explicit SerialManager(QObject *parent = nullptr);
  ~SerialManager();
  bool open(const QString &port_name, QSerialPort::BaudRate baudrate,
            QSerialPort::DataBits dataBits = QSerialPort::Data8,
            QSerialPort::Parity parity = QSerialPort::NoParity,
            QSerialPort::StopBits stopBits = QSerialPort::OneStop,
            QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
  void close();
  void sendCmd(const char *data, qint64 size);

private slots:
  void recvData();

private:
  QSerialPort *serial_;

  QFile csvFile_;
  QString fileName_;

  uint8_t Res_ = 0;                     // 读取到的每一字节数据
  int16_t USART_RX_STA_ = 0;            // 接收状态标志
  uint8_t USART_RX_BUF_[USART_REC_LEN]; // 接收缓冲区
};

#pragma pack(1)
struct MotorFBData {
  short vel[3];
  int pos[3];
};
#pragma pack()
} // namespace serial_manager
