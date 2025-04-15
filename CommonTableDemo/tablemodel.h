#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QHeaderView>
#include <QCheckBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "simulatedataoperation.h"

#define CHECK_BOX_COLUMN 0

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);

    // 实现QAbstractTableModel的虚函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    //插入行
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    //删除行
    bool removeRows(int row, int count, const QModelIndex &parent) override;
//    bool setTableData(const QVector<QVector<QString>>& data);
    //给model传入data
    bool setTableData(const QVector<TableData>& data);
    //给model传入表头
    void setTableHeaderData(const QStringList& headerData);
    //设置第一列是否为checkbox
    void setFirstColumnCheckBox(bool state);
    //设置某行的checkbox选中状态
    void setCheckBoxSelect(const int& row, bool state);
    //设置表格是否可以编辑
    void setTableisEditable(bool editable);
    void onStateChanged();
    //插入行
    bool insertRowData(int row, int count, const QModelIndex &parent, TableData data);
    //更新指定行数据
    void updateData(int row, TableData data);
    //获取一行的数据
    TableData getOneRowData(int row);
    //获取所有数据
    QVector<TableData> getALLRowData();
    //上移一行
    void moveRowUp(int row);
    //下移一行
    void moveRowDown(int row);
public slots:
    void onStateChanged(int state);
signals:
    void stateChanged(int);

    void signalRowChecked(int row, bool bchecked);

    void signalSelectAll();

    void signalDeSelectALL();
private:
    QStringList m_headerData;
    bool m_bEnableCheckBox;
    bool m_bEditable;
    int m_nfirstcolumn;
//    QList<bool> m_bCheckedlist;
//    QVector<QVector<QString>> m_data; // 表格数据
    QVector<TableData> m_tabledata; //传入的表格数据模版不同格式
};

class TableHeaderView:public QHeaderView
{
    Q_OBJECT
public:
    TableHeaderView(Qt::Orientation orientation, QWidget *parent);
    ~TableHeaderView();

public:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    bool event(QEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
public slots:
    void onStateChanged(int state);
signals:
    void stateChanged(int);

public:
    bool m_bPressed;
    bool m_bChecked;
    bool m_bTristate;
    bool m_bNoChange;
    bool m_bMoving;
};

#endif // TABLEMODEL_H
