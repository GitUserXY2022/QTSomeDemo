#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QMouseEvent>
#include <QKeyEvent>

class TableView : public QTableView {
    Q_OBJECT

public:
    TableView(QWidget *parent = nullptr) : QTableView(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        // 禁用某些鼠标事件，例如左键点击
//        if (event->button() == Qt::LeftButton) {
//            event->ignore();
//            return;
//        }
        // 禁用 Shift + 鼠标左键等组合键
        if ((event->modifiers() == Qt::ShiftModifier) && (event->button() == Qt::LeftButton)) {
            event->ignore();
            return;
        }
        QTableView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        // 禁用鼠标移动事件导致的选中
        event->ignore();
    }

    void keyPressEvent(QKeyEvent *event) override {
        // 禁用 Shift + 鼠标左键等组合键
//        if ((event->modifiers() == Qt::ShiftModifier) && (event->button() == Qt::LeftButton)) {
//            event->ignore();
//            return;
//        }
        QTableView::keyPressEvent(event);
    }
};
#endif // TABLEVIEW_H
