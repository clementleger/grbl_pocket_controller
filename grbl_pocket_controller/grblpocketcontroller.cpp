#include "grblpocketcontroller.h"
#include "ui_grblpocketcontroller.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFileDialog>


GrblPocketController::GrblPocketController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GrblPocketController),
    serialPort(nullptr)
{
    ui->setupUi(this);

    /* Fill baudrates */
    QList<qint32> baudrates = QSerialPortInfo::standardBaudRates();
    QList<qint32>::iterator i;
    int index = 0;
    for (i = baudrates.end() - 1; i >= baudrates.begin(); i--, index++) {
        ui->settings_baudrate->addItem(QString::number(*i));
        if (*i == 115200)
            ui->settings_baudrate->setCurrentIndex(index);
    }

    refresh_serial_ports();
}

GrblPocketController::~GrblPocketController()
{
    delete ui;
}

void GrblPocketController::refresh_serial_ports()
{
    while (ui->settings_serial->count()) {
        ui->settings_serial->removeItem(0);
    }

    /* Then fill ports */
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator port;

    for (port = ports.begin(); port != ports.end(); port++) {
        ui->settings_serial->addItem(port->portName(), QVariant::fromValue(*port));
    }
}

void GrblPocketController::set_connect_state(bool connected)
{

    ui->settings_baudrate->setEnabled(!connected);
    ui->settings_serial->setEnabled(!connected);
    ui->settings_refresh_serial->setEnabled(!connected);
    ui->tab_control->setEnabled(connected);
    ui->tab_gcode->setEnabled(connected);
    ui->tab_send->setEnabled(connected);
    QString status = connected ? "Connected to " : "Disconnected";
    if (connected) {
        status.append(serialPort->portName());
    }
    ui->status_bar->showMessage(status);
}

void GrblPocketController::on_settings_refresh_serial_clicked()
{
    refresh_serial_ports();
}

void GrblPocketController::on_settings_connect_clicked()
{
    if (serialPort == nullptr) {
        int index = ui->settings_serial->currentIndex();
        QVariant variant = ui->settings_serial->itemData(index);
        QSerialPortInfo portInfo = qvariant_cast<QSerialPortInfo>(variant);
        qInfo() << "Connecting to" << portInfo.systemLocation();

        serialPort = new QSerialPort(portInfo);
        QString baudrate =  ui->settings_baudrate->itemText(index);
        serialPort->setBaudRate(baudrate.toInt());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        bool ret = serialPort->open(QIODevice::ReadWrite);
        if (!ret) {
            ui->status_bar->showMessage(serialPort->errorString());
            delete serialPort;
            serialPort = nullptr;
            return;
        }

        ui->settings_connect->setText(tr("Disconnect"));
        set_connect_state(true);

    } else {
        serialPort->close();
        delete serialPort;
        serialPort = nullptr;
        ui->settings_connect->setText(tr("Connect"));
        qInfo() << "Disconnecting serial port";
        set_connect_state(false);
    }
}

void GrblPocketController::on_action_quit_triggered()
{
    QApplication::quit();
}

void GrblPocketController::on_load_gcode_clicked()
{
    QString gcodeFile = QFileDialog::getSaveFileName(this,
            tr("Load GCode file"), "",
            tr("GCode (*.gcode);;All Files (*)"));
}
