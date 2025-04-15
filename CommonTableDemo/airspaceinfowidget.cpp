#include "airspaceinfowidget.h"
#include "ui_airspaceinfowidget.h"
#include <QStyleOption>
#include <QDesktopWidget>
#include <QToolTip>
#include <QDebug>

QMyTableViewBtnDelegate::QMyTableViewBtnDelegate(QStringList btnNames, QWidget *parent)
    : QStyledItemDelegate(parent),
      m_btnNames(btnNames)
{
}

QMyTableViewBtnDelegate::~QMyTableViewBtnDelegate()
{

}

// 绘制按钮
void QMyTableViewBtnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    // 计算按钮显示区域
    int nCount = m_btnNames.count();

    int w = nCount != 0 ? option.rect.width() / nCount : 0;
    if(w < 0) {
        return;
    }

    for (int i = 0; i < nCount; ++i)
    {
        // 绘制按钮
        QStyleOptionButton button;
        button.rect =  option.rect.adjusted(5 + i*w , 5, -(w * ( nCount - i -1 ) + 5)  , -8);
//        qDebug() << i << "adjusted" << 8 + i*w << 8 << -(w * ( nCount - i -1 ) + 2)  << -8;
        //        button.text = m_btnNames.at(i);
        button.state |= QStyle::State_Enabled;

        if (button.rect.contains(m_mousePoint))
        {
            if (m_nType == 0)
            {
                button.state |= QStyle::State_MouseOver;
            }
            else if (m_nType == 1)
            {
                button.state |= QStyle::State_Sunken;
            }
        }
        QPushButton pushBtn;
        if (i == nCount - 1)
        {
            pushBtn.setStyleSheet("QPushButton{border-width: 0px;\
                                  position: absolute;\
                    left: 0px;\
top: 0px;\
            max-width: 15px;\
width:15px;\
height: 15px;\
background: transparent;\
            background-image: url(:/images/delete.png);\
            background-repeat: no-repeat;\
            subcontrol-position: center;\
            border-radius: 7.5px;\
        }\
QPushButton:hover{background-color:#081421; color:#FFFFFF;}");
    }
    else
    {
        pushBtn.setStyleSheet("QPushButton{border-width: 0px;\
                              position: absolute;\
                left: 0px;\
top: 0px;\
        max-width: 15px;\
width:15px;\
height: 15px;\
background: transparent;\
        background-image: url(:/images/edit.png);\
        background-repeat: no-repeat;\
        subcontrol-position: center;\
        border-radius: 7.5px;\
        font-size: 11px;\
color: #000000;}\
QPushButton:hover{background-color:#081421; color:#FFFFFF;}");
}

pushBtn.style()->drawControl(QStyle::CE_PushButton, &button, painter, &pushBtn);
}
}

// 响应按钮事件 - 划过、按下
bool QMyTableViewBtnDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    m_nType = -1;
    bool bRepaint = false;
    QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
    m_mousePoint = pEvent->pos();

    int nCount = m_btnNames.count();

    int w = nCount != 0 ? option.rect.width() / nCount : 0;
    if(w < 0) {
        return false;
    }

    // 还原鼠标样式
    QApplication::restoreOverrideCursor();

    for (int i = 0; i < nCount; ++i)
    {
        QStyleOptionButton button;
        button.rect =  option.rect.adjusted(5 + i*w , 5, -(w * ( nCount - i -1 ) + 5)  , -8);

        // 鼠标位于按钮之上
        if (!button.rect.contains(m_mousePoint))
            continue;

        bRepaint = true;
        switch (event->type())
        {
        // 鼠标滑过
        case QEvent::MouseMove:
        {
            // 设置鼠标样式为手型
            QApplication::setOverrideCursor(Qt::PointingHandCursor);

            m_nType = 0;

            QToolTip::showText(pEvent->globalPos(), m_btnNames.at(i));
            break;
        }
            // 鼠标按下
        case QEvent::MouseButtonPress:
        {
            m_nType = 1;
            break;
        }
            // 鼠标释放
        case QEvent::MouseButtonRelease:
        {
            switch (i)
            {
            case 0:
            {
                emit editData(index);
                break;
            }
            case 1:
            {
                emit deleteData(index);
                break;
            }
            default:
                break;
            }
        }
        default:
            break;
        }
    }

    return bRepaint;
}

AirSpaceInfoWidget::AirSpaceInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AirSpaceInfoWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("testairspaceinfo"));

    // 设置间隔行背景颜色
    ui->tableviewAirSpaceinfo->setAlternatingRowColors(true);
    ui->tableviewAirSpaceinfo->setStyleSheet("QTableView { alternate-background-color: #f0f0f0; background-color: #ffffff; }");

    // 设置样式
    ui->tableviewAirSpaceinfo->setStyleSheet("QTableView,QTableWidget{\
                                             padding: 0px;\
                                             qproperty-showGrid: false;\
                                             alternate-background-color: #344659; /*rgba(255, 255, 255, 0.1);*/\
                                             selection-color: #344659;\
                                             color: white;\
                                             }\
                                             \
                                             .QTableView,.QTableWidget{\
                                             min-height:30px;\
                                             padding: 2px;\
                                             border: none;\
                                             border-radius: 3px;\
                                             background-color: #081421; /*rgba(255, 255, 255, 0.05);*/\
                                             }\
                                             \
                                             QTableView::item,QTableWidget::item{\
                                             border:none;\
                                             background:transparent;\
                                             padding:2px;\
                                             margin:0px;\
                                             }\
                                             QTableView::item:hover, QTableWidget::item:hover\
                                             {\
                                             border-radius: 1px;\
                                             color: #04EAFB;\
                                             border: transparent;\
                                             }\
                                             \
                                             QTableView::item:selected, QTableWidget::item:selected {\
                                             border-radius: 1px;\
                                             color: #04EAFB;\
                                             }\
                                             \
                                             /*QTableView 左上角样式*/\
                                             QTableView QTableCornerButton::section {\
                                             color: red;\
                                             background-color: #081421;\
                                             border: none;\
                                             }\
                                             \
                                             /*表头*/\
                                             QHeaderView {\
                                             border: none;\
                                             border-radius:0px;\
                                             background-color: #344659;\
                                             \
                                             color: white;\
                                             }\
                                             QHeaderView::section {\
                                             padding: 5px;\
                                             border: none;\
                                             border-radius:0px;\
                                             background-color: #344659;\
                                             \
                                             color: white;\
                                             }"
                                             );

    // 创建模型
    model = new CustomTableModel(this); // 假设有6行6列
    // 初始化数据
    std::vector<std::vector<QVariant>> data = {
        {1, "Alice", "Math", 8,""},
        {2, "Bob", "Science", 90,""},
        {3, "Charlie", "History", 88,""},
        {4, "Alice", "Math", 85,""},
        {5, "Bob", "Science", 90,""},
        {6, "Charlie", "History", 88,""}
    };
    model->setTableData(data);

    QStringList columnHeaders; // 填充你的列标题
    // ... 填充 columnHeaders ...
    columnHeaders << "column1" << "column2" << "column3" << "column4" << "" ;
    model->setTableHeaderData(columnHeaders);

    //根据时间排序功能
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::DisplayRole);
    ui->tableviewAirSpaceinfo->setModel(proxyModel);
//    ui->tableviewAirSpaceinfo->setSortingEnabled(true);
    // 连接表头的sectionClicked信号到一个槽函数以处理排序
    ui->tableviewAirSpaceinfo->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    ui->tableviewAirSpaceinfo->horizontalHeader()->setSortIndicatorShown(true);

    //自定义表头
    CustomHeaderView *headerView = new CustomHeaderView(Qt::Horizontal, ui->tableviewAirSpaceinfo);
    ui->tableviewAirSpaceinfo->setHorizontalHeader(headerView);

    // 连接自定义头部视图的排序信号到表格视图的排序功能
    connect(headerView, &CustomHeaderView::sortRequested, [=](Qt::SortOrder order) {
        proxyModel->sort(1, order); // assuming time column is at index 1
        // 恢复选中状态
        selectionModel->clearSelection();
    });

    //去掉行头
    ui->tableviewAirSpaceinfo->verticalHeader()->setVisible(false);
    //每行自适应宽度
    ui->tableviewAirSpaceinfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // 设置代理
    // 构造函数中传入按钮列表即可添加任意个按钮
    QMyTableViewBtnDelegate *m_btnDelegate = new QMyTableViewBtnDelegate(QStringList()<<"edit"<<"delete", this);
    // 必须要设置此项，否则样式表的hover无法生效
    ui->tableviewAirSpaceinfo->setMouseTracking(true);
    // 为指定列设置代理
    ui->tableviewAirSpaceinfo->setItemDelegateForColumn(4, m_btnDelegate);
    ui->tableviewAirSpaceinfo->setColumnWidth(4, 50); // 注意索引从0开始，所以第五列是索引4
    // 连接信号槽，根据需要添加
    //    connect(m_btnDelegate, &QMyTableViewBtnDelegate::editData, this, [=](const QModelIndex &index){
    //        QMessageBox::information(this, "提示", QString("编辑第 %1").arg(index.row()+1));
    //    });
    connect(m_btnDelegate, &QMyTableViewBtnDelegate::editData, this, &AirSpaceInfoWidget::onEditButtonClicked);
    connect(m_btnDelegate, &QMyTableViewBtnDelegate::deleteData, this, [=](const QModelIndex &index){
        QMessageBox::information(this, "提示", QString("删除第 %1").arg(index.row()+1));
    });

    m_Delegate = new AirSpaceCustomDelegate();
    ui->tableviewAirSpaceinfo->setItemDelegateForColumn(0, m_Delegate);
    ui->tableviewAirSpaceinfo->setItemDelegateForColumn(1, m_Delegate);
    ui->tableviewAirSpaceinfo->setItemDelegateForColumn(2, m_Delegate);
    ui->tableviewAirSpaceinfo->setItemDelegateForColumn(3, m_Delegate);

//    ui->tableviewAirSpaceinfo->setAlternatingRowColors(true);
//    ui->tableviewAirSpaceinfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 创建选择模型
    selectionModel = new QItemSelectionModel(model);
    ui->tableviewAirSpaceinfo->setSelectionModel(selectionModel);
    // 连接选择更改信号到槽函数
    connect(ui->tableviewAirSpaceinfo->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AirSpaceInfoWidget::onSelectionChanged);

}

AirSpaceInfoWidget::~AirSpaceInfoWidget()
{
    delete ui;
}

//void AirSpaceInfoWidget::setAirSpaceInfoTableText(int row, int col, const QString & text)
//{
//    // if (row < 0 || row >= ui->tableWidgetAirSpaceinfo->rowCount()
//    //     || col < 0 || col >= ui->tableWidgetAirSpaceinfo->columnCount())
//    //     return;
//    auto item = ui->tableWidgetAirSpaceinfo->item(row, col);
//    qDebug() << "item";
//    if (!item) {
//        item = new QTableWidgetItem(text);
//        ui->tableWidgetAirSpaceinfo->setItem(row, col, item);
//    }
//    item->setText(text);
//    item->setTextAlignment(Qt::AlignHCenter);
//    qDebug() << "text" << text;
//    // ui->tableWidgetAirSpaceinfo->setItem(row, col, item);

//    QPushButton *editButton = new QPushButton("Edit");
//    connect(editButton, &QPushButton::clicked, [=](){
//        QMessageBox::information(nullptr, "Edit", "Editing row" + QString::number(row));
//    });
//    ui->tableWidgetAirSpaceinfo->setCellWidget(row, 4, editButton);

//    QPushButton *deleteButton = new QPushButton("Delete");
//    connect(deleteButton, &QPushButton::clicked, [=](){
//        ui->tableWidgetAirSpaceinfo->removeRow(row);
//    });
//    ui->tableWidgetAirSpaceinfo->setCellWidget(row, 5, deleteButton);
//}

//void AirSpaceInfoWidget::reloadAirSpaceInfoWithButtons()
//{
//    // int rowCount = ui->tableWidgetAirSpaceinfo->rowCount();
//    // ui->tableWidgetAirSpaceinfo->insertRow(rowCount);
//    int column = ui->tableWidgetAirSpaceinfo->columnCount();
//    qDebug() << "########tableWidgetAirSpaceinfo column" <<  column;
//    //
//    for (int i = 0; i < 6; i++) {
//        ui->tableWidgetAirSpaceinfo->insertRow(i);
//        for(int j = 0; j <= 3; j++){
//            qDebug() << QString("addtestData %1 %2").arg(i).arg(j) ;
//            setAirSpaceInfoTableText(i, j, QString("testData %1 %2").arg(i).arg(j));
//        }
//    }
//}

void AirSpaceInfoWidget::onEditButtonClicked(const QModelIndex &index)
{
    // 如果当前正在编辑，切换回不可编辑状态
    if (m_currentindexrow == index.row()) {
        // 设置第一行的第1、2、3列的单元格为选中状态
        QModelIndex editIndex = proxyModel->index(index.row(), 1);
        ui->tableviewAirSpaceinfo->edit(editIndex);
        for (int column : {1, 2, 3}) {
            QModelIndex index = proxyModel->index(index.row(), column);
            selectionModel->select(index, QItemSelectionModel::Select);
        }
        return;
    }

    m_Delegate->setEditableRows(index.row());
    // 假设我们有一个按钮用于切换的可编辑状态
    m_Delegate->changeEditableStatus(true);

    m_currentindexrow = index.row();
    //选中编辑的这一行
    ui->tableviewAirSpaceinfo->selectRow(index.row());
    // 设置第一行的第1、2、3列的单元格为选中状态
    QModelIndex editIndex = proxyModel->index(index.row(), 1);
    ui->tableviewAirSpaceinfo->edit(editIndex);
    for (int column : {1, 2, 3}) {
        QModelIndex index = proxyModel->index(index.row(), column);
        selectionModel->select(index, QItemSelectionModel::Select);
    }
    // （可选）更新视图以反映更改
    ui->tableviewAirSpaceinfo->viewport()->update();
}

void AirSpaceInfoWidget::onDeleteButtonClicked(const QModelIndex &index)
{
    qDebug() << "onDeleteButtonClicked";
}

void AirSpaceInfoWidget::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << "onSelectionChanged";

    std::vector<int> numbers = {1, 2, 3};

    // 使用std::remove将数字2移动到末尾
    auto newEnd = std::remove(numbers.begin(), numbers.end(), 2);

    // 使用erase擦除被移动的元素
    numbers.erase(newEnd, numbers.end());

    if (!selected.indexes().isEmpty()) {
        qDebug() << "selected.indexes().first().column()"<< selected.indexes().first().column();
        int selectedrow = selected.indexes().first().row();
        int selectedcloumn = selected.indexes().first().column();
        QModelIndex editIndex = proxyModel->index(selectedrow, selectedcloumn);
        ui->tableviewAirSpaceinfo->edit(editIndex);
        for (int column : numbers ) {
            QModelIndex editIndex = proxyModel->index(selectedrow, selectedcloumn);
            qDebug() << editIndex;
            ui->tableviewAirSpaceinfo->edit(editIndex);
        }
    }else{
        qDebug() << "!selected.indexes().isEmpty()";
        for (int column : {1, 2, 3}) {
            QModelIndex editIndex = proxyModel->index(m_currentindexrow, column);
            selectionModel->select(editIndex, QItemSelectionModel::Select);
        }
    }
}
