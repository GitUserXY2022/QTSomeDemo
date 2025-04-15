#ifndef CHOOSETABLE_H
#define CHOOSETABLE_H

#include <QWidget>
#include "controlsectorwidget.h"
#include "airspaceinfowidget.h"
namespace Ui {
class ChooseTable;
}

class ChooseTable : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseTable(QWidget *parent = nullptr);
    ~ChooseTable();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ChooseTable *ui;
};

#endif // CHOOSETABLE_H
