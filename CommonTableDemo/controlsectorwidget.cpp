#include "controlsectorwidget.h"
#include "ui_controlsectorwidget.h"

#include "simulatedataoperation.h"
ControlSectorWidget::ControlSectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlSectorWidget)
{
    ui->setupUi(this);
    // 初始化表格数据 这里只是创建的测试数据，
    //最后是由Simulatedataoperation这个类从数据库中读取并转换为相应数据

    QVector<TableData> m_tabledata; // 表格数据
//    m_tabledata.resize(3000);
//    for (int i = 0; i < 3000; ++i) {
//        m_tabledata[i].m_bChecked = false;
//        m_tabledata[i].m_data.resize(5);
//        for (int j = 0; j < 5; ++j) {
//            m_tabledata[i].m_data[j] = QString("Row %1, Column %2").arg(i).arg(j);
//        }
//    }

    SimulateDataOperation testdata;
    m_tabledata = testdata.getDataFromDB("tabledemo_info1");

    QStringList header;
    header.append("第一列");
    header.append("第二列");
    header.append("第三列");
    header.append("第四列");
    header.append("第五列");

    //初始化表格，需要传入数据和表头
    ui->ControlSectorTablewidget->setTableData(m_tabledata);
    ui->ControlSectorTablewidget->setTableHeader(header);
    //启用单选框一列
    ui->ControlSectorTablewidget->enableCheckBox(true);
    //启用编辑功能
    ui->ControlSectorTablewidget->enableEditable(true);

    //完成 初始化表格
    ui->ControlSectorTablewidget->setupTableWidget();

    //设置行高列宽
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->resizeSection(0,30);
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->resizeSection(1,100);
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->resizeSection(2,100);
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->resizeSection(3,100);
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->resizeSection(4,100);
    ui->ControlSectorTablewidget->gettableView()->horizontalHeader()->resizeSection(5,100);
    for(int row = 0; row < 20; ++row)
    {
        ui->ControlSectorTablewidget->gettableView()->verticalHeader()->resizeSection(row, 25);
    }
}

ControlSectorWidget::~ControlSectorWidget()
{
    delete ui;
}

void ControlSectorWidget::on_DeleteBtn_clicked()
{
    qDebug() << "Delete Hang:" <<  ui->ControlSectorTablewidget->deleteTableRows(ui->ControlSectorTablewidget->getSelectedRows());
}

void ControlSectorWidget::on_AddBtn_clicked()
{
    //弹出新增窗口

    //要添加的数据初始化
    TableData newdata;
    newdata.m_bChecked = false;
    newdata.m_data.resize(4);
    newdata.m_data.append("add");
    ui->ControlSectorTablewidget->clearAllSelectedRows();
    //要添加的行的位置,默认为最底部
    int addrow = ui->ControlSectorTablewidget->getTableViewRow();
    qDebug() << "add one :" << ui->ControlSectorTablewidget->insertTableRows(addrow, newdata);
    ui->ControlSectorTablewidget->gettableView()->verticalHeader()->resizeSection(addrow, 25);
    //tableView滚动到最底部显示
    ui->ControlSectorTablewidget->gettableView()->scrollToBottom();
}

void ControlSectorWidget::on_CopyBtn_clicked()
{
    //插入复制数据
    qDebug() << "copy one :" << ui->ControlSectorTablewidget->copyTableRows(ui->ControlSectorTablewidget->getSelectedRows());
}

void ControlSectorWidget::on_EditBtn_clicked()
{
    //仅支持一行的编辑
    if(ui->ControlSectorTablewidget->getSelectedRows().size() > 1)
    {
        qDebug() << "您已选择多行，请选择一行进行编辑！";
        return;
    }
    //先取出要编辑的行数据
    int editRow = ui->ControlSectorTablewidget->getSelectedRows().at(0);
    TableData editData = ui->ControlSectorTablewidget->getTableRowData(editRow);
    //弹出编辑窗口

    editData.m_data.replace(0, "editchange");
    //将改变好的行数据传入更新表格
    ui->ControlSectorTablewidget->editTableRow(editRow, editData);

}

void ControlSectorWidget::on_MoveUPBtn_clicked()
{
    //获取选中的行
    QList<int> moveuprows = ui->ControlSectorTablewidget->getSelectedRows();
    if(moveuprows.size() > 1)
    {
        qDebug() << "不能同时移动多行，请选择一行进行移动！";
        ui->ControlSectorTablewidget->clearAllSelectedRows();
        return;
    }
    ui->ControlSectorTablewidget->moveTableRowUp(moveuprows.at(0));
}

void ControlSectorWidget::on_MoveDownBtn_clicked()
{
    QList<int> movedownrow = ui->ControlSectorTablewidget->getSelectedRows();
    if(movedownrow.size() > 1)
    {
        qDebug() << "不能同时移动多行，请选择一行进行移动！";
        ui->ControlSectorTablewidget->clearAllSelectedRows();
        return;
    }
    ui->ControlSectorTablewidget->moveTableRowDown(movedownrow.at(0));
}

void ControlSectorWidget::on_SreachBtn_clicked()
{
    ui->ControlSectorTablewidget->sreachTableRows(ui->lineEdit->text());
}
