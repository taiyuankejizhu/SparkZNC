#include "clearscreen.h"
#include "ui_clearscreen.h"

ClearScreen::ClearScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClearScreen)
{
    ui->setupUi(this);
}

ClearScreen::~ClearScreen()
{
    delete ui;
}
