#ifndef GRBLPOCKETCONTROLLER_H
#define GRBLPOCKETCONTROLLER_H

#include <QMainWindow>

namespace Ui {
class GrblPocketController;
}

class GrblPocketController : public QMainWindow
{
    Q_OBJECT

public:
    explicit GrblPocketController(QWidget *parent = nullptr);
    ~GrblPocketController();

private:
    Ui::GrblPocketController *ui;
};

#endif // GRBLPOCKETCONTROLLER_H
