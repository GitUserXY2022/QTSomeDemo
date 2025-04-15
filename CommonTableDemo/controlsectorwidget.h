#ifndef CONTROLSECTORWIDGET_H
#define CONTROLSECTORWIDGET_H

#include <QWidget>

namespace Ui {
class ControlSectorWidget;
}

class ControlSectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlSectorWidget(QWidget *parent = 0);
    ~ControlSectorWidget();

private slots:
    void on_DeleteBtn_clicked();

    void on_AddBtn_clicked();

    void on_CopyBtn_clicked();

    void on_EditBtn_clicked();

    void on_MoveUPBtn_clicked();

    void on_MoveDownBtn_clicked();

    void on_SreachBtn_clicked();

private:
    Ui::ControlSectorWidget *ui;
};

#endif // CONTROLSECTORWIDGET_H
