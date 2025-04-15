#ifndef TABLESOURCEMODEL_H
#define TABLESOURCEMODEL_H

#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QHeaderView>
#include <QCheckBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define CHECK_BOX_COLUMN 0

//后续数据的传入，可以通过模版来控制
//template<typename T>
class TableData
{
//   explicit TableData(const int& column){
//        m_column = column;
//    }
public:
    //第一列的复选框参数
    bool m_bChecked;
    //数据，后期数据可以通过模板数据类或别的结构进来
    QVector<QString> m_data;
};

class TableSourceModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableSourceModel(QObject *parent = nullptr);

    // 实现QAbstractTableModel的虚函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

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
    //第一列复选框状态变更
    void onStateChanged();

    //更新指定行数据
    void updateData(int row, TableData data);
    //获取一行的数据
    TableData getOneRowData(int row);
    //获取所有数据
    QVector<TableData> getALLRowData();

public slots:
    void onStateChanged(int state);
signals:
    void stateChanged(int);

    void signalRowChecked(int row, bool bchecked);

private:
    QStringList m_headerData;
    bool m_bEnableCheckBox;
    bool m_bEditable;
    int m_nfirstcolumn;
    QVector<TableData> m_tabledata; //传入的表格数据模版不同格式
};



#endif // TABLESOURCEMODEL_H
