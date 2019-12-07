/**
  * This class ends the game.
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson, and Theaux Mas.
  */

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
