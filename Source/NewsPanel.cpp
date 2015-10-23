#include "NewsPanel.h"
#include "ui_newspanel.h"

NewsPanel::NewsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewsPanel)
{
    ui->setupUi(this);
}

NewsPanel::~NewsPanel()
{
    delete ui;
}
