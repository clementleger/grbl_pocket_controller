#ifndef GCODECONTROLLER_H
#define GCODECONTROLLER_H

#include <QSerialPort>
#include <QSerialPortInfo>

class GCodeController
{
public:
    GCodeController();
    void set_serial_port(const QSerialPortInfo &info);
    void set_baudrate(int baudrate);
    void send_gcode_line(QString gcode);
    void open_serial();

private:
    QSerialPort mSerialPort;
};

#endif // GCODECONTROLLER_H
