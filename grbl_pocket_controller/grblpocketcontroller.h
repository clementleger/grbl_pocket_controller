#ifndef GRBLPOCKETCONTROLLER_H
#define GRBLPOCKETCONTROLLER_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QShortcut>
#include <gcodecontroller.h>

#define JOG_TAB_INDEX 2

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
    Ui::GrblPocketController *mUi;
    QSerialPort *mSerialPort;
    QShortcut *mShortcutUp;
    QShortcut *mShortcutDown;
    QShortcut *mShortcutRight;
    QShortcut *mShortcutLeft;
    GCodeController mGcodeController;

    void refresh_serial_ports();
    void set_connect_state(bool connected);
    void send_gcode_command(QString command, QString &return_value);
    void enable_control_shortcuts(bool enabled);

private slots:
    void on_settings_connect_clicked();
    void on_settings_refresh_serial_clicked();
    void on_action_quit_triggered();
    void on_gcode_load_clicked();
    void on_send_command_button_clicked();
    void on_tabs_currentChanged(int index);
    void on_control_move_y_min_clicked();
    void on_control_move_y_max_clicked();
    void on_control_move_x_min_clicked();
    void on_control_move_x_max_clicked();
};

#endif // GRBLPOCKETCONTROLLER_H
