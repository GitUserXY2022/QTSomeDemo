#include "tablesourcemodel.h"

#include "TableSourceModel.h"


TableSourceModel::TableSourceModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_nfirstcolumn = 0;
    m_bEnableCheckBox = false;
    m_bEditable = false;
}

int TableSourceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //    return m_data.size();
    return m_tabledata.size();
}

int TableSourceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //    return m_data.isEmpty() ? 0 : m_data.first().size()+m_nfirstcolumn;
    return m_tabledata.isEmpty() ? 0 : m_tabledata.first().m_data.size() + m_nfirstcolumn;
}

QVariant TableSourceModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(orientation != Qt::Horizontal)
        return QVariant("only support horizontal");

    if(role != Qt::DisplayRole)
        return QVariant();

    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);// 表头内容位置
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            if (m_bEnableCheckBox && section == CHECK_BOX_COLUMN)
                return QStringLiteral("");
            //                return QStringLiteral("全选");

            if(section-m_nfirstcolumn >= m_headerData.size())
                return QVariant("NoName");

            return m_headerData.at(section-m_nfirstcolumn);
        }
    }
    default:
        return QVariant();
    }

    return QVariant();
}

QVariant TableSourceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    //    bool bChecked = m_bCheckedlist.at(nRow);
    bool bChecked = m_tabledata.at(nRow).m_bChecked;

    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::white);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);// 表格数据内容位置
    case Qt::DisplayRole:
    {
        if (m_bEnableCheckBox && nColumn == CHECK_BOX_COLUMN)
            return QVariant();
        if(nColumn != CHECK_BOX_COLUMN)
        {
            //            qDebug() << "TableSourceModel::data" << m_data[index.row()][index.column()-m_nfirstcolumn];
            //            return m_data[index.row()][index.column()-m_nfirstcolumn];
//            qDebug() << "TableSourceModel::data" << m_tabledata[index.row()].m_data[index.column()-m_nfirstcolumn];
            return m_tabledata[index.row()].m_data[index.column()-m_nfirstcolumn];
        }
    }
    case Qt::CheckStateRole:
    {
        if (m_bEnableCheckBox && nColumn == CHECK_BOX_COLUMN)
            return bChecked ? Qt::Checked : Qt::Unchecked;
    }
    default:
        return QVariant();
    }

    return QVariant();
}

bool TableSourceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    int nColumn = index.column();
    TableData tabledata = m_tabledata.at(index.row());
    switch(role)
    {
    case Qt::DisplayRole:
    {
        if(nColumn != CHECK_BOX_COLUMN)
        {
            //m_data[index.row()][index.column()-m_nfirstcolumn] = value.toString();;
            tabledata.m_data[index.column()-m_nfirstcolumn] = value.toString();
            emit dataChanged(index, index);
            return true;
        }
    }
    case Qt::CheckStateRole:
    {
        if(m_bEnableCheckBox && nColumn == CHECK_BOX_COLUMN)
        {
            //            bool bchecked = (value.toInt() == Qt::Checked);
            //            qDebug() << "case Qt::CheckStateRole:" << bchecked;
            tabledata.m_bChecked = (value.toInt() == Qt::Checked);
            m_tabledata.replace(index.row(), tabledata);
            emit dataChanged(index, index);
            qDebug() << "di" << index.row()+1<< "hang" << "checkboxstatus:" << m_tabledata.at(index.row()).m_bChecked;
            emit signalRowChecked(index.row(), tabledata.m_bChecked);
            onStateChanged();
            return true;
        }
    }
    default:
        return false;
    }
    return false;

}

Qt::ItemFlags TableSourceModel::flags(const QModelIndex &index) const
{
    //    if (!index.isValid())
    //        return Qt::NoItemFlags;

    //    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    if(!m_bEditable)
    {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    if (m_bEnableCheckBox && index.column() == CHECK_BOX_COLUMN)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}


bool TableSourceModel::setTableData(const QVector<TableData> &data)
{
    //checkdata
    beginResetModel();
    m_tabledata = data;
    endResetModel();
    return 0;
}

void TableSourceModel::setTableHeaderData(const QStringList &headerData)
{
    m_headerData = headerData;
}

void TableSourceModel::setFirstColumnCheckBox(bool state)
{
    if(state)
    {
        m_nfirstcolumn = 1;
    }
    else
    {
        m_nfirstcolumn = 0;
    }
    m_bEnableCheckBox = state;
}

void TableSourceModel::setCheckBoxSelect(const int &row, bool state)
{
    TableData data = m_tabledata.at(row);
    data.m_bChecked = state;
    updateData(row, data);
    onStateChanged();
}

void TableSourceModel::setTableisEditable(bool editable)
{
    m_bEditable = editable;
}

void TableSourceModel::onStateChanged()
{
    int checked = 0, unchecked = 0;
    //    foreach (bool bChecked, m_bCheckedlist) {
    //    if(bChecked)
    //        checked++;
    //    else
    //        unchecked++;
    //}
    //if(checked == m_bCheckedlist.count())
    //    emit stateChanged(Qt::Checked);
    //else if(unchecked == m_bCheckedlist.count())
    //    emit stateChanged(Qt::Unchecked);
    //else
    //    emit stateChanged(Qt::PartiallyChecked);
    foreach (TableData oneRowTabledata, m_tabledata) {
        if(oneRowTabledata.m_bChecked)
            checked++;
        else
            unchecked++;
    }
    if(checked == m_tabledata.count())
        emit stateChanged(Qt::Checked);
    else if(unchecked == m_tabledata.count())
        emit stateChanged(Qt::Unchecked);
    else
        emit stateChanged(Qt::PartiallyChecked);
}



void TableSourceModel::updateData(int row, TableData data)
{
    if(row >= 0 && row < m_tabledata.size())
    {
        m_tabledata.replace(row, data);

        //发送数据变化信号，通知视图更新
        QModelIndex topLeft = createIndex(row, 0);
        QModelIndex bottomRight = createIndex(row, columnCount() - 1);
        emit dataChanged(topLeft, bottomRight);
    }
}

TableData TableSourceModel::getOneRowData(int row)
{
    return m_tabledata.at(row);
}

QVector<TableData> TableSourceModel::getALLRowData()
{
    return m_tabledata;
}


void TableSourceModel::onStateChanged(int state)
{
//    qDebug() << "TableSourceModel::onStateChanged";
    state==Qt::Checked?Qt::Checked:Qt::Unchecked;//判断全选是选中还是未选中
    QModelIndex index;
    //    for (int i = 0; i < m_bCheckedlist.count(); ++i)
    //    {
    //        index = this->index(i, 0);
    //        setData(index, state, Qt::CheckStateRole);//使用自己重写的setData更新状态
    //    }
    for (int i = 0; i < m_tabledata.count(); ++i)
    {
        index = this->index(i, 0);
        setData(index, state, Qt::CheckStateRole);//使用自己重写的setData更新状态
    }
}


