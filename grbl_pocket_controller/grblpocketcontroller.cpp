#include "grblpocketcontroller.h"
#include "ui_grblpocketcontroller.h"
#include <QSerialPort>
#include <QSerialPortInfo>

GrblPocketController::GrblPocketController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GrblPocketController)
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

    /* Then fill ports */
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator port;

    for (port = ports.begin(); port != ports.end(); port++) {
        ui->settings_serial->addItem(port->portName());
    }
}

GrblPocketController::~GrblPocketController()
{
    delete ui;
}

void GrblPocketController::on_settings_connect_clicked()
{

}
