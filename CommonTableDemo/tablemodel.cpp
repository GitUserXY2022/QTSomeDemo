#include "tablemodel.h"


TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_nfirstcolumn = 0;
    m_bEnableCheckBox = false;
    m_bEditable = false;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //    return m_data.size();
    return m_tabledata.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //    return m_data.isEmpty() ? 0 : m_data.first().size()+m_nfirstcolumn;
    return m_tabledata.isEmpty() ? 0 : m_tabledata.first().m_data.size() + m_nfirstcolumn;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant TableModel::data(const QModelIndex &index, int role) const
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
            //            qDebug() << "TableModel::data" << m_data[index.row()][index.column()-m_nfirstcolumn];
            //            return m_data[index.row()][index.column()-m_nfirstcolumn];
//            qDebug() << "TableModel::data" << m_tabledata[index.row()].m_data[index.column()-m_nfirstcolumn];
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

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    int nColumn = index.column();
    TableData tabledata = m_tabledata.at(index.row());
    switch(role)
    {
    case Qt::EditRole:
    {
        qDebug() << "Qt::EditRole";
        // 更新内部数据结构
        if(nColumn != CHECK_BOX_COLUMN)
        {
            qDebug() << "Qt::EditRole" << value;
            tabledata.m_data[index.column()-m_nfirstcolumn] = value.toString();
            m_tabledata.replace(index.row(), tabledata);
            emit dataChanged(index, index);
            return true;
        }
    }
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

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
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

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //如果插入零行，则返回false，表示插入失败
    if(count == 0) return false;

    //没有父类
    if(!parent.isValid())
    {
        //从row开始插入行， 知道row + count -1 处
        beginInsertRows(QModelIndex(), row, row + count - 1);
    }else{
        //
        beginInsertRows(parent, row, row + count - 1);
    }
    //对model中存储的数据做相应的修改
    for(int addCount = 0; addCount < count; addCount++)
    {
        TableData emptydata;
        emptydata.m_bChecked = false;
        m_tabledata.insert(row + addCount, emptydata);
    }

    endInsertRows();

    emit layoutChanged();

    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    //如果删除零行，则返回false，表示删除行失败
    if(count == 0) return false;

    //没有父类
    if(!parent.isValid())
    {
        //从row开始删除行， 知道row + count -1 处
        beginRemoveRows(QModelIndex(), row, row + count - 1);
    }else{
        //
        beginRemoveRows(parent, row, row + count - 1);
    }

    //对model中存储的数据做相应的修改
    for(int removeCount = 0; removeCount < count; removeCount++)
    {
        m_tabledata.removeAt(row + removeCount);
    }

    endRemoveRows();

    emit layoutChanged();

    return true;

}


//bool TableModel::setTableData(const QVector<QVector<QString> > &data)
//{
//    //checkdata
//    m_data = data;
//    for(int i = 0; i < m_data.size(); i++)
//    {
//        m_bCheckedlist.append(false);
//    }
//    return 0;
//}

bool TableModel::setTableData(const QVector<TableData> &data)
{
    //checkdata
    beginResetModel();
    m_tabledata = data;
    endResetModel();
    return 0;
}

void TableModel::setTableHeaderData(const QStringList &headerData)
{
    m_headerData = headerData;
}

void TableModel::setFirstColumnCheckBox(bool state)
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

void TableModel::setCheckBoxSelect(const int &row, bool state)
{
    TableData data = m_tabledata.at(row);
    data.m_bChecked = state;
    updateData(row, data);
    onStateChanged();
}

void TableModel::setTableisEditable(bool editable)
{
    m_bEditable = editable;
}

void TableModel::onStateChanged()
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

bool TableModel::insertRowData(int row, int count, const QModelIndex &parent, TableData data)
{
    //如果插入零行，则返回false，表示插入失败
    if(count == 0) return false;
//    if(count != data.count())
//    {
//        qDebug() << "插入的行数与数据不匹配";
//        return false;
//    }

    //没有父类
    if(!parent.isValid())
    {
        //从row开始插入行， 知道row + count -1 处
        beginInsertRows(QModelIndex(), row, row + count - 1);
    }else{
        //
        beginInsertRows(parent, row, row + count - 1);
    }
    //对model中存储的数据做相应的修改
    for(int addCount = 0; addCount < count; addCount++)
    {
        m_tabledata.insert(row + addCount, data);
        for (int i = 0; i < m_tabledata.size(); i++) {
            qDebug() << "insertRowData.m_tabledata.size()" << i << m_tabledata.size() << m_tabledata.at(i).m_data;
        }
    }

    endInsertRows();

    emit layoutChanged();

    return true;
}

void TableModel::updateData(int row, TableData data)
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

TableData TableModel::getOneRowData(int row)
{
    return m_tabledata.at(row);
}

QVector<TableData> TableModel::getALLRowData()
{
    return m_tabledata;
}

void TableModel::moveRowUp(int row)
{
    if(row >= 0 && row < m_tabledata.size())
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
        std::swap(m_tabledata[row], m_tabledata[row - 1]);
        endMoveRows();
    }
}

void TableModel::moveRowDown(int row)
{
    if(row >= 0 && row < m_tabledata.size() - 1)
    {
        beginMoveRows(QModelIndex(), row + 1, row + 1, QModelIndex(), row);
        std::swap(m_tabledata[row], m_tabledata[row + 1]);
        endMoveRows();
    }
}


void TableModel::onStateChanged(int state)
{
//    qDebug() << "TableModel::onStateChanged";
    state==Qt::Checked?Qt::Checked:Qt::Unchecked;//判断全选是选中还是未选中
    if(state == Qt::Checked)
    {
        emit signalSelectAll();
    }else if(state == Qt::Unchecked)
    {
        emit signalDeSelectALL();
    }

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

TableHeaderView::TableHeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent),
      m_bPressed(false),
      m_bChecked(false),
      m_bTristate(false),
      m_bNoChange(false),
      m_bMoving(false)
{
    setSectionsClickable(true);
}

TableHeaderView::~TableHeaderView()
{

}

// 槽函数，用于更新复选框状态
void TableHeaderView::onStateChanged(int state)
{
//    qDebug() << " TableHeaderView::onStateChanged" ;
    if (state == Qt::PartiallyChecked)
    {
        m_bTristate = true;
        m_bNoChange = true;
    }
    else
    {
        m_bNoChange = false;
    }
    m_bChecked = (state != Qt::Unchecked);
    update();
}



// 绘制复选框
void TableHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();
    if (logicalIndex == CHECK_BOX_COLUMN)
    {
        QStyleOptionButton option;
        option.initFrom(this);

        if (m_bChecked)
            option.state |= QStyle::State_Sunken;

        if (m_bTristate && m_bNoChange)
            option.state |= QStyle::State_NoChange;
        else
            option.state |= m_bChecked ? QStyle::State_On : QStyle::State_Off;
        if (testAttribute(Qt::WA_Hover) && underMouse()) {
            if (m_bMoving)
                option.state |= QStyle::State_MouseOver;
            else
                option.state &= ~QStyle::State_MouseOver;
        }

        QCheckBox checkBox;
        option.rect = QRect(4,5,15,15);//绘制复选框的位置与大小
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, &checkBox);
    }
}

// 鼠标按下表头
void TableHeaderView::mousePressEvent(QMouseEvent *event)
{
    int nColumn = logicalIndexAt(event->pos());
    if ((event->buttons() & Qt::LeftButton) && (nColumn == CHECK_BOX_COLUMN))
    {
        m_bPressed = true;
    }
    else
    {
        QHeaderView::mousePressEvent(event);
    }
}

// 鼠标从表头释放，发送信号，更新model数据
void TableHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bPressed)
    {
        if (m_bTristate && m_bNoChange)
        {
            m_bChecked = true;
            m_bNoChange = false;
        }
        else
        {
            m_bChecked = !m_bChecked;
        }

        update();

        Qt::CheckState state = m_bChecked ? Qt::Checked : Qt::Unchecked;
        emit stateChanged(state);
    }
    else
    {
        QHeaderView::mouseReleaseEvent(event);
    }

    m_bPressed = false;
}



// 鼠标滑过、离开，更新复选框状态
bool TableHeaderView::event(QEvent *event)
{
    updateSection(0);
    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave)
    {
        QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
        int nColumn = logicalIndexAt(pEvent->x());
        if (nColumn == CHECK_BOX_COLUMN)
        {
            m_bMoving = (event->type() == QEvent::Enter);
            update();
            return true;
        }
    }
    return QHeaderView::event(event);
}
