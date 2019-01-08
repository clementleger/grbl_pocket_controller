#ifndef GRBLPOCKETCONTROLLER_H
#define GRBLPOCKETCONTROLLER_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>

namespace Ui {
class GrblPocketController;
}

Q_DECLARE_METATYPE(QSerialPortInfo)

class GrblPocketController : public QMainWindow
{
    Q_OBJECT

public:
    explicit GrblPocketController(QWidget *parent = nullptr);
    ~GrblPocketController();

private:
    Ui::GrblPocketController *ui;
    QSerialPort *serialPort;
    void refresh_serial_ports();
    void set_connect_state(bool connected);

private slots:
    void on_settings_connect_clicked();
    void on_settings_refresh_serial_clicked();
    void on_action_quit_triggered();
    void on_load_gcode_clicked();
};

#endif // GRBLPOCKETCONTROLLER_H
