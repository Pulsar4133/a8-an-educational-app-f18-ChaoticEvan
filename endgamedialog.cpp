#include "endgamedialog.h"
#include "ui_endgamedialog.h"

endgamedialog::endgamedialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::endgamedialog)
{
    ui->setupUi(this);
}

endgamedialog::~endgamedialog()
{
    delete ui;
}
