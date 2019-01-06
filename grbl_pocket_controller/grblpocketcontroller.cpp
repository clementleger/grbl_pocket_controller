#include "grblpocketcontroller.h"
#include "ui_grblpocketcontroller.h"

GrblPocketController::GrblPocketController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GrblPocketController)
{
    ui->setupUi(this);
}

GrblPocketController::~GrblPocketController()
{
    delete ui;
}
