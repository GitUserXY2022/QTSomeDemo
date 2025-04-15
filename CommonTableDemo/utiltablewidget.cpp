#include "utiltablewidget.h"

UtilTableWidget::UtilTableWidget(QWidget *parent) :
    QWidget(parent)
{
    m_tableview = new TableView(this);
    m_tablemodel = new TableModel(this);
    m_tableDelegate = new TableDelegate(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    layout->addWidget(m_tableview);
    SimulateDataOperation aa;
}

UtilTableWidget::~UtilTableWidget()
{
    delete m_tableview;
    delete m_proxyModel;
    delete m_tablemodel;
    delete m_tableDelegate;
}

void UtilTableWidget::setupTableWidget()
{
//    m_proxyModel = new QSortFilterProxyModel(this);
//    m_proxyModel->setSourceModel(m_tablemodel);
//    m_tableview->setModel(m_proxyModel);
    m_tableview->setModel(m_tablemodel);
    m_selectionModel = m_tableview->selectionModel();
    m_tableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableview->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableview->setItemDelegate(m_tableDelegate);
    m_tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableview->verticalHeader()->setVisible(false);
    m_tableview->setAlternatingRowColors(true);
    m_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(m_tablemodel, &TableModel::signalRowChecked, this, &UtilTableWidget::onSelectRow);
    connect(m_selectionModel, &QItemSelectionModel::selectionChanged, this, &UtilTableWidget::onselectionModelChangeSelection);
    connect(m_tablemodel, &TableModel::signalSelectAll, this, &UtilTableWidget::onSelectALLRows);
    connect(m_tablemodel, &TableModel::signalDeSelectALL, this, &UtilTableWidget::onDeSelectALLRows);
//    connect(m_selectionModel, &QItemSelectionModel::currentRowChanged, this, &UtilTableWidget::onCurrentSelectRowChange);
}

void UtilTableWidget::setTableHeader(const QStringList& horizontalHeader)
{
    m_tablemodel->setTableHeaderData(horizontalHeader);
}

void UtilTableWidget::setTableData(const QVector<TableData>& data)
{
    m_tablemodel->setTableData(data);
}

QTableView *UtilTableWidget::gettableView()
{
    return m_tableview;
}

void UtilTableWidget::enableCheckBox(bool state)
{
    // 设置垂直表头代理
    //    m_headerview = new SCheckBoxHeaderView(10, Qt::Horizontal, m_tableview);
    //    m_tableview->setHorizontalHeader(m_headerview);
    //    connect(m_headerview, &SCheckBoxHeaderView::checkStausChange, this, [=](bool status){
    //        //控制所有目标的显示与隐藏
    //        for (int i = 0; i < m_tablemodel->rowCount(); i++) {
    //            QModelIndex index = m_tablemodel->index(i, 0);
    //            m_tablemodel->setData(index, status, Qt::CheckStateRole);
    //        }
    //    });
    m_headerview = new TableHeaderView(Qt::Horizontal, this);
    // 设置表头
    m_tableview->setHorizontalHeader(m_headerview);
    m_tablemodel->setFirstColumnCheckBox(state);
    // 关联表头复选框与第一列复选框
    connect(m_headerview, SIGNAL(stateChanged(int)), m_tablemodel, SLOT(onStateChanged(int)));
    connect(m_tablemodel, SIGNAL(stateChanged(int)), m_headerview, SLOT(onStateChanged(int)));
}

void UtilTableWidget::enableEditable(bool state)
{
    m_tablemodel->setTableisEditable(state);
}

int UtilTableWidget::getTableViewRow()
{
    return m_tablemodel->rowCount();
}

int UtilTableWidget::getTableViewColumn()
{
    return m_tablemodel->columnCount();
}

TableData UtilTableWidget::getTableRowData(int row)
{
    return m_tablemodel->getOneRowData(row);
}

QVector<TableData> UtilTableWidget::getTableAllData()
{
    return m_tablemodel->getALLRowData();
}

bool UtilTableWidget::insertTableRows(const int& insertRow, const TableData& newRowData)
{
    QModelIndex insertindex = m_tablemodel->index(insertRow, 0);
    bool insertresult = m_tablemodel->insertRowData(insertRow, 1, insertindex, newRowData);
//    m_selectionModel->clearSelection();
    selectRow(insertRow);
    m_tableview->scrollTo(insertindex);
    return insertresult;
}

bool UtilTableWidget::deleteTableRows(const QList<int> &deleterows)
{
    bool deleteresult = true;
    QList<int> sortdeleterows = deleterows;
    //删除行的时候要从大往小的编号进行删除，因为删除一行就会更新表格数据
    for(int i = sortdeleterows.size() - 1; i >= 0; --i)
    {
        int deleterow = sortdeleterows[i];
        if(deleterow < 0 || deleterow >= m_tablemodel->rowCount())
            continue;

        qDebug() << "删除的行： " << deleterows[i];
        QModelIndex deleteindex = m_tablemodel->index(deleterow, 0);
        if(!m_tablemodel->removeRows(deleterow, 1, deleteindex))
        {
            deleteresult = false;
        }

        //调整后续行的索引,防止还有更大的删除行，就需要进行调整
        for(int j = i + 1; j < sortdeleterows.size(); ++j)
        {
            if(sortdeleterows[j] > deleterow)
                sortdeleterows[j]--;
        }
    }
//    m_selectionModel = m_tableview->selectionModel();
    clearAllSelectedRows();
    return deleteresult;
}

void UtilTableWidget::editTableRow(const int &editRow, const TableData &editRowData)
{
    m_tablemodel->updateData(editRow, editRowData);
}

bool UtilTableWidget::copyTableRows(const QList<int> &copyrows)
{
    clearAllSelectedRows();
    bool copyresult = true;
    //需要先将所有需要复制的数据拷贝一份
    QVector<TableData> vecCopydata;
    for(int i = 0; i < copyrows.size(); i++)
    {
        vecCopydata.append(m_tablemodel->getOneRowData(copyrows.at(i)));
    }
    //开始插入
    for(int i = 0; i < copyrows.size(); i++)
    {
        qDebug() << "need copy row:" << copyrows.at(i);
        if(!insertTableRows((copyrows.at(i) + i + 1), vecCopydata.at(i)))
        {
            copyresult = false;
        }
    }
    return copyresult;
}

void UtilTableWidget::moveTableRowUp(int row)
{
    clearAllSelectedRows();
    m_tablemodel->moveRowUp(row);
    selectRow(row - 1);
}

void UtilTableWidget::moveTableRowDown(int row)
{
    clearAllSelectedRows();
    m_tablemodel->moveRowDown(row);
    selectRow(row + 1);
}

void UtilTableWidget::clearAllSelectedRows()
{
    m_selectionModel->clearSelection();
//    m_selectionModel = m_tableview->selectionModel();
//    disconnect(m_selectionModel, &QItemSelectionModel::selectionChanged, this, &UtilTableWidget::onselectionModelChangeSelection);
//    connect(m_selectionModel, &QItemSelectionModel::selectionChanged, this, &UtilTableWidget::onselectionModelChangeSelection);
}

QList<int> UtilTableWidget::getSelectedRows()
{
    QModelIndexList selectedIndexes = m_selectionModel->selectedRows();

    if(selectedIndexes.size() == 0)
    {
        qDebug() << "getSelectedRows clearAllSelectedRows" ;
        clearAllSelectedRows();
    }
    QList<int> listSelectRows;
    foreach (const QModelIndex &index, selectedIndexes) {
        int row = index.row();
        qDebug() << "Selected rowCounts:" << selectedIndexes.size() <<"Selected row:" << row;
        listSelectRows.append(row);
    }
    std::sort(listSelectRows.begin(), listSelectRows.end());
    return listSelectRows;
}

void UtilTableWidget::selectRow(const int &row)
{
    qDebug() << "need selected row:" << row;

//    QModelIndex proxyIndex =  m_tablemodel->index(row, 0);
//    QModelIndex topleft = m_proxyModel->mapFromSource(proxyIndex);
//    QModelIndex proxyIndex1 =  m_tablemodel->index(row, m_tablemodel->columnCount() - 1);
//    QModelIndex bottomRight = m_proxyModel->mapFromSource(proxyIndex1);

    QModelIndex topleft = m_tablemodel->index(row, 0);
    QModelIndex bottomRight = m_tablemodel->index(row, m_tablemodel->columnCount() - 1);

//    QModelIndex topleft = m_proxyModel->index(row, 0);
//    QModelIndex bottomRight = m_proxyModel->index(row, m_proxyModel->columnCount() - 1);

    m_selectionModel->select(QItemSelection(topleft, bottomRight), QItemSelectionModel::Select);

    if(!m_listSelectRows.contains(row))
    {
        m_listSelectRows.append(row);
    }

//    updateCheckboxbySelectRows();
}

void UtilTableWidget::deSelectRow(const int &row)
{
    qDebug() << "need Deselect row:" << row;

//    QModelIndex topleft = m_tablemodel->index(row, 0);
//    QModelIndex bottomRight = m_tablemodel->index(row, m_tablemodel->columnCount() - 1);

//    m_selectionModel->select(QItemSelection(topleft, bottomRight), QItemSelectionModel::Deselect);

    m_listSelectRows.removeAll(row);

    updateCheckboxbySelectRows();

}

void UtilTableWidget::selectRows(const QList<int>& rows)
{
    foreach (int row, rows) {
        QModelIndex topleft = m_tablemodel->index(row, 0);
        QModelIndex bottomRight = m_tablemodel->index(row, m_tablemodel->columnCount() - 1);
//        qDebug() << "Selected rowCounts:" << rows.size() <<"Selected row:" << row;
        m_selectionModel->select(QItemSelection(topleft, bottomRight), QItemSelectionModel::Select);
    }

//    updateCheckboxbySelectRows();
}

void UtilTableWidget::updateCheckboxbySelectRows()
{
    QList<int> temp = m_listSelectRows;
//    m_selectionModel->clearSelection();
    clearAllSelectedRows();
    for(int i = 0; i < temp.size(); i++)
    {
        qDebug() << "updateCheckboxbySelectRows m_listSelectRows:" << temp.size();
        m_tablemodel->setCheckBoxSelect(temp.at(i), true);
        QModelIndex topleft = m_tablemodel->index(temp.at(i), 0);
        QModelIndex bottomRight = m_tablemodel->index(temp.at(i), m_tablemodel->columnCount() - 1);

//        QModelIndex topleft = m_proxyModel->index(temp.at(i), 0);
//        QModelIndex bottomRight = m_proxyModel->index(temp.at(i), m_proxyModel->columnCount() - 1);

        m_selectionModel->select(QItemSelection(topleft, bottomRight), QItemSelectionModel::Select);
    }
}

void UtilTableWidget::sreachTableRows(const QString &searchText)
{
    if(firstsreach)
    {
        // 先保存一份之前的所有行数据，转为读数据库的方式就不需要了，可以从数据库重新拉数据
        m_oldData = m_tablemodel->getALLRowData();
        firstsreach = false;
    }

    if(searchText == NULL)
    {
        m_tablemodel->setTableData(m_oldData);
        firstsreach = true;
        return;
    }

    int rowCount = m_tablemodel->rowCount();
    QList<int> matchingRows; // 存储匹配行的索引
    QVector<TableData> tempsreachRow;
    // 遍历所有行来查找匹配项
    for (int row = 0; row < rowCount; ++row) {
        bool isMatch = false;
        for (int col = 0; col < m_tablemodel->columnCount(); ++col) {
            QModelIndex index = m_tablemodel->index(row, col);
            QString cellText = m_tablemodel->data(index, Qt::DisplayRole).toString();
            qDebug() << "hideNonMatchingRows" << row << searchText << cellText;
            if (cellText.contains(searchText, Qt::CaseInsensitive)) {
                isMatch = true;
                break; // 找到匹配项，跳出内层循环
            }
        }
        if (isMatch) {
            qDebug() << "匹配的行为：" << row;
            matchingRows.append(row); // 添加匹配行的索引到列表
            tempsreachRow.append(m_tablemodel->getOneRowData(row)); //取出所有匹配行的数据暂存
        }
    }

    //
    m_tablemodel->setTableData(tempsreachRow);

    // 更新视图以反映变化
    m_tableview->viewport()->update();
}

void UtilTableWidget::onFilterChanged(const QString &text)
{
    if (text.isEmpty())
    {
        QMessageBox::information(this, "提示", "搜索内容为空");
        m_proxyModel->setFilterFixedString(NULL);
        return;
    }
    m_proxyModel->setFilterFixedString(text);
}

void UtilTableWidget::onSelectRow(int row, bool bchecked)
{
    qDebug() << "onSelectRow" << row << bchecked;
    if(bchecked)
    {
       selectRow(row);
    }else{
       deSelectRow(row);
    }
}

void UtilTableWidget::onSelectALLRows()
{
    qDebug() << "onSelectALLRows";
    qDebug() << disconnect(m_selectionModel, &QItemSelectionModel::selectionChanged, this, &UtilTableWidget::onselectionModelChangeSelection);
    qDebug() << disconnect(m_tablemodel, &TableModel::signalRowChecked, this, &UtilTableWidget::onSelectRow);

    int rowCount = m_tablemodel->rowCount();
    // 创建一个范围，从第一行第一列到最后一行最后一列
    QItemSelection selection(m_tablemodel->index(0, 0),
                              m_tablemodel->index(rowCount - 1, m_tablemodel->columnCount() - 1));

    // 清除任何现有的选择
    clearAllSelectedRows();

    // 选中新范围
    m_selectionModel->select(selection, QItemSelectionModel::Select | QItemSelectionModel::Rows);

    qDebug() << "onSelectALLRows m_listSelectRows size:" << m_listSelectRows.size();

    for (int i = 0; i < rowCount; i++) {
        if(!m_listSelectRows.contains(i))
        {
            m_listSelectRows.append(i);
        }
    }
}

void UtilTableWidget::onDeSelectALLRows()
{
    qDebug() << "onDeSelectALLRows";
    connect(m_selectionModel, &QItemSelectionModel::selectionChanged, this, &UtilTableWidget::onselectionModelChangeSelection);
    connect(m_tablemodel, &TableModel::signalRowChecked, this, &UtilTableWidget::onSelectRow);
    for(int i = 0; i < m_listSelectRows.size(); i++)
    {
        qDebug() << "onDeSelectALLRows selected m_listSelectRows:" << m_listSelectRows.at(i);
    }
    // 清除任何现有的选择
    clearAllSelectedRows();
    m_listSelectRows.clear();
    qDebug() << "onDeSelectALLRows m_listSelectRows size:" << m_listSelectRows.size();
}

void UtilTableWidget::onselectionModelChangeSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "onselectionModelChangeSelection" ;
    foreach (QItemSelectionRange index, selected) {
        qDebug() << "onselectionModelChangeSelection selected" << index.topLeft().row() << index.bottomRight().row();
        if(index.topLeft().row() < m_tablemodel->rowCount())
        {
             m_tablemodel->setCheckBoxSelect(index.topLeft().row(), true);
        }
        if(!m_listSelectRows.contains(index.topLeft().row()))
        {
            m_listSelectRows.append(index.topLeft().row());
        }
    }
//    for(int i = 0; i < m_listSelectRows.size(); i++)
//    {
//        qDebug() << "onselectionModelChangeSelection selected m_listSelectRows:" << m_listSelectRows.at(i);
//    }
    foreach (QItemSelectionRange index, deselected) {
        qDebug() << "onselectionModelChangeSelection deselected" << index.topLeft().row() << index.bottomRight().row();
        if(index.topLeft().row() < m_tablemodel->rowCount())
        {
             m_tablemodel->setCheckBoxSelect(index.topLeft().row(), false);
        }
        m_listSelectRows.removeAll(index.topLeft().row());
    }
//    for(int i = 0; i < m_listSelectRows.size(); i++)
//    {
//        qDebug() << "onselectionModelChangeSelection deselected m_listSelectRows:" << m_listSelectRows.at(i);
//    }
}

void UtilTableWidget::onCurrentSelectRowChange(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug() << "onCurrentSelectRowChange selected" << current.row() << previous.row();
}
