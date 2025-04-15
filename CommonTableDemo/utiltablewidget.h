#ifndef UTILTABLEWIDGET_H
#define UTILTABLEWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDebug>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QMouseEvent>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QModelIndexList>
#include "tablemodel.h"
#include "tabledelegate.h"
#include "tableview.h"
#include "simulatedataoperation.h"
//class SCheckBoxHeaderView : public QHeaderView
//{
//    Q_OBJECT
//private:
//    bool isChecked;
//    int m_checkColIdx;
//public:
//    SCheckBoxHeaderView(int checkColumnIndex, Qt::Orientation orientation, QWidget * parent = 0) :
//        QHeaderView(orientation, parent) {
//        m_checkColIdx = checkColumnIndex;
//        isChecked = false;
//    }
//signals:
//    void checkStausChange(bool);
//protected:
//    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
//        painter->save();
//        QHeaderView::paintSection(painter, rect, logicalIndex);
//        painter->restore();
//        if (logicalIndex == m_checkColIdx) {
//            QStyleOptionButton option;
//            QCheckBox *checkBox = new QCheckBox(dynamic_cast<QWidget*>(this->parent()));
//            // QRect(x, y, width, height) 参数分别指定 checkBox 的位置(x,y), 大小(width， height)
//            // 注: 大小会偏大，高13显示为正常15的高度
//            option.rect = QRect(6, 6, 12, 12);
//            if (isChecked)
//                option.state = QStyle::State_On;
//            else
//                option.state = QStyle::State_Off;
//            this->style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, checkBox);
//        }
//    }
//    void mousePressEvent(QMouseEvent *event) {
//        if (visualIndexAt(event->pos().x()) == m_checkColIdx) {
//            isChecked = !isChecked;
//            this->updateSection(m_checkColIdx);
//            emit checkStausChange(isChecked);
//        }
//        QHeaderView::mousePressEvent(event);
//    }
//};


class UtilTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UtilTableWidget(QWidget *parent = 0);
    ~UtilTableWidget();

    void setupTableWidget();
    void setTableHeader(const QStringList& horizontalHeader);
    void setTableData(const QVector<TableData>& data);
    QTableView *gettableView();
    //是否启动第一列为checkbox
    void enableCheckBox(bool state);
    //是否启用编辑
    void enableEditable(bool state);
    int getTableViewRow();
    int getTableViewColumn();
    //获取某行数据
    TableData getTableRowData(int row);
    //获取表格所有数据
    QVector<TableData> getTableAllData();
    //新增
    void addTableData();
    //插入
    bool insertTableRows(const int& insertRow, const TableData& newRowData);
    //删除
    bool deleteTableRows(const QList<int>& deleterows);
    //编辑
    void editTableRow(const int& editRow, const TableData& editRowData);
    //复制
    bool copyTableRows(const QList<int>& copyrows);
    //上移
    void moveTableRowUp(int row);
    //下移
    void moveTableRowDown(int row);
    //取消

    //清除选中行
    void clearAllSelectedRows();
    //获取选中的行
    QList<int> getSelectedRows();
    //选中一行
    void selectRow(const int& row);
    //取消选中一行
    void deSelectRow(const int& row);
    //选中多行
    void selectRows(const QList<int>& rows);
    //根据某行是否选中更新复选框是否选中
    void updateCheckboxbySelectRows();

    //搜索
    void sreachTableRows(const QString &searchText);

public slots:
    void onFilterChanged(const QString& text);
    void onSelectRow(int row, bool bchecked);
    void onSelectALLRows();
    void onDeSelectALLRows();
    void onselectionModelChangeSelection(const QItemSelection &selected, const QItemSelection &deselected);
    void onCurrentSelectRowChange(const QModelIndex &current, const QModelIndex &previous);
private:
    TableView *m_tableview;
    TableModel *m_tablemodel;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    TableDelegate *m_tableDelegate;
    TableHeaderView *m_headerview;
    QList<int> m_listSelectRows;
    QVector<TableData>  m_oldData;
    bool firstsreach = true;
};

#endif // UTILTABLEWIDGET_H
