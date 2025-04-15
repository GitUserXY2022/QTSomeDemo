#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QComboBox>
#include <QItemDelegate>
// 自定义表格项代理，使表格项可编辑
class TableDelegate : public QStyledItemDelegate {

public:
    TableDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};
#endif // TABLEDELEGATE_H
