#include "choosetable.h"
#include "ui_choosetable.h"

ChooseTable::ChooseTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseTable)
{
    ui->setupUi(this);
}

ChooseTable::~ChooseTable()
{
    delete ui;
}

void ChooseTable::on_pushButton_2_clicked()
{
    AirSpaceInfoWidget *airSpaceInfoWidget = new AirSpaceInfoWidget;
    airSpaceInfoWidget->resize(1200,1000);
    airSpaceInfoWidget->show();
}


void ChooseTable::on_pushButton_clicked()
{
    ControlSectorWidget *controlSectorWidget = new ControlSectorWidget;
    controlSectorWidget->show();
}

