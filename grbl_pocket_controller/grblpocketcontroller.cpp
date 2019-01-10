#include "grblpocketcontroller.h"
#include "ui_grblpocketcontroller.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QShortcut>

GrblPocketController::GrblPocketController(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::GrblPocketController),
    mSerialPort(nullptr)
{

    mUi->setupUi(this);

    mShortcutUp = new QShortcut(QKeySequence("Up"), this);
    QObject::connect(mShortcutUp, SIGNAL(activated()), mUi->control_move_y_min, SLOT(click()));
    mShortcutDown = new QShortcut(QKeySequence("Down"), this);
    QObject::connect(mShortcutDown, SIGNAL(activated()), mUi->control_move_y_max, SLOT(click()));
    mShortcutLeft = new QShortcut(QKeySequence("Left"), this);
    QObject::connect(mShortcutLeft, SIGNAL(activated()), mUi->control_move_x_max, SLOT(click()));
    mShortcutRight = new QShortcut(QKeySequence("Right"), this);
    QObject::connect(mShortcutRight, SIGNAL(activated()), mUi->control_move_x_min, SLOT(click()));
    enable_control_shortcuts(false);

    /* Fill baudrates */
    QList<qint32> baudrates = QSerialPortInfo::standardBaudRates();
    QList<qint32>::iterator i;
    int index = 0;
    for (i = baudrates.end() - 1; i >= baudrates.begin(); i--, index++) {
        mUi->settings_baudrate->addItem(QString::number(*i));
        if (*i == 115200)
            mUi->settings_baudrate->setCurrentIndex(index);
    }

    refresh_serial_ports();
}

void GrblPocketController::enable_control_shortcuts(bool enabled)
{
    mShortcutUp->setEnabled(enabled);
    mShortcutDown->setEnabled(enabled);
    mShortcutLeft->setEnabled(enabled);
    mShortcutRight->setEnabled(enabled);
}

GrblPocketController::~GrblPocketController()
{
    delete mUi;
}

void GrblPocketController::refresh_serial_ports()
{
    while (mUi->settings_serial->count()) {
        mUi->settings_serial->removeItem(0);
    }

    /* Then fill ports */
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator port;

    for (port = ports.begin(); port != ports.end(); port++) {
        mUi->settings_serial->addItem(port->portName(), QVariant::fromValue(*port));
    }
}

void GrblPocketController::set_connect_state(bool connected)
{

    mUi->settings_baudrate->setEnabled(!connected);
    mUi->settings_serial->setEnabled(!connected);
    mUi->settings_refresh_serial->setEnabled(!connected);
    mUi->settings_controller->setEnabled(!connected);
    mUi->tab_jog->setEnabled(connected);
    mUi->tab_gcode->setEnabled(connected);
    mUi->tab_send->setEnabled(connected);
    QString status = connected ? "Connected to " : "Disconnected";
    if (connected) {
        status.append(mSerialPort->portName());
    }
    mUi->status_bar->showMessage(status);
}

void GrblPocketController::on_settings_refresh_serial_clicked()
{
    refresh_serial_ports();
}

void GrblPocketController::on_settings_connect_clicked()
{
    if (mSerialPort == nullptr) {
        int index = mUi->settings_serial->currentIndex();
        QVariant variant = mUi->settings_serial->itemData(index);
        QSerialPortInfo portInfo = qvariant_cast<QSerialPortInfo>(variant);
        qInfo() << "Connecting to" << portInfo.systemLocation();

        mGcodeController.set_serial_port(portInfo);
        int baudrate =  mUi->settings_baudrate->itemText(index).toInt();
        mGcodeController.set_baudrate(baudrate);

        bool ret = mSerialPort->open(QIODevice::ReadWrite);
        if (!ret) {
            mUi->status_bar->showMessage(mSerialPort->errorString());
            delete mSerialPort;
            mSerialPort = nullptr;
            return;
        }

        mUi->settings_connect->setText(tr("Disconnect"));
        set_connect_state(true);

    } else {
        mSerialPort->close();
        delete mSerialPort;
        mSerialPort = nullptr;
        mUi->settings_connect->setText(tr("Connect"));
        qInfo() << "Disconnecting serial port";
        set_connect_state(false);
    }
}

void GrblPocketController::on_action_quit_triggered()
{
    QApplication::quit();
}

void GrblPocketController::on_gcode_load_clicked()
{
    QString gcodeFile = QFileDialog::getSaveFileName(this,
            tr("Load GCode file"), "",
            tr("GCode (*.gcode);;All Files (*)"));

}

void GrblPocketController::on_send_command_button_clicked()
{
    QString read;
    QString gcode_line = mUi->send_command_line->text();
    if (gcode_line.isEmpty())
        return;

    qInfo() << "Sending command" << gcode_line;
    mGcodeController.send_gcode_line(gcode_line);
    qInfo() << read;
}

void GrblPocketController::on_tabs_currentChanged(int index)
{
    if (index == JOG_TAB_INDEX) {
        enable_control_shortcuts(true);
    } else {
        enable_control_shortcuts(false);
    }
}

void GrblPocketController::on_control_move_y_min_clicked()
{
    printf("y_min clicked\n");
}

void GrblPocketController::on_control_move_y_max_clicked()
{
    printf("y_max clicked\n");
}

void GrblPocketController::on_control_move_x_min_clicked()
{
    printf("x_min clicked\n");
}

void GrblPocketController::on_control_move_x_max_clicked()
{
    printf("x_max clicked\n");
}
