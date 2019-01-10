#include "gcodecontroller.h"

GCodeController::GCodeController():
    mSerialPort()
{

}

void GCodeController::set_serial_port(const QSerialPortInfo &info)
{
    mSerialPort.setPort(info);
}

void GCodeController::set_baudrate(int baudrate)
{
    mSerialPort.setBaudRate(baudrate);
}

void GCodeController::open_serial()
{
    mSerialPort.setDataBits(QSerialPort::Data8);
    mSerialPort.setParity(QSerialPort::NoParity);
    mSerialPort.setStopBits(QSerialPort::OneStop);
    mSerialPort.setFlowControl(QSerialPort::NoFlowControl);
}

void GCodeController::send_gcode_line(QString command)
{
    QByteArray cmdData(command.toLocal8Bit());
    cmdData.append("\r\n");
    const qint64 bytesWritten = mSerialPort.write(cmdData);

    if (bytesWritten == -1) {
        return;
    } else if (bytesWritten != cmdData.size()) {
        return;
    } else if (!mSerialPort.waitForBytesWritten(100)) {
        return;
    }

    QByteArray readData = mSerialPort.readAll();
    while (mSerialPort.waitForReadyRead(100))
        readData.append(mSerialPort.readAll());
}
