#ifndef AIRSPACEINFOWIDGET_H
#define AIRSPACEINFOWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QHeaderView>
#include <QPushButton>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QMouseEvent>
#include <QPainter>
#include <QDateTimeEdit>
#include <QList>
#include <QSortFilterProxyModel>
#include <QStyleOptionHeader>
#include <vector>
#include <QItemSelectionModel>
//自定义模型
class CustomTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    CustomTableModel(QObject *parent = nullptr): QAbstractTableModel(parent){}
    // CustomTableModel(int rows, int columns, QObject *parent = nullptr)
    //     : QStandardItemModel(parent) {
    //     // 初始化数据
    //     m_data = {
    //         {1, "Alice", "Math", 8,""},
    //         {2, "Bob", "Science", 90,""},
    //         {3, "Charlie", "History", 88,""},
    //         {4, "Alice", "Math", 85,""},
    //         {5, "Bob", "Science", 90,""},
    //         {6, "Charlie", "History", 88,""}
    //     };
    // }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return m_data.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return m_data[0].size();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
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
                // if (m_bEnableCheckBox && section == CHECK_BOX_COLUMN)
                //     return QStringLiteral("");
                // //                return QStringLiteral("全选");

                // if(section-m_nfirstcolumn >= m_headerData.size())
                //     return QVariant("NoName");

                return m_headerData.at(section);
            }
        }
        default:
            return QVariant();
        }

        return QVariant();
    }
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return m_data[index.row()][index.column()];
        }
        return QVariant();
    }

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override {
        if (role == Qt::EditRole) {
            m_data[index.row()][index.column()] = value;
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
            return true;
        }
        return false;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override {
        if (index.column() == 1 || index.column() == 2 || index.column() == 3) {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        }
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    void setTableHeaderData(const QStringList& data){
        m_headerData = data;
    }

    void setTableData(const std::vector<std::vector<QVariant>>& data){
        m_data = data;
    }
private:
    std::vector<std::vector<QVariant>> m_data;
    QStringList m_headerData;
};
//自定义表头类,在表头增加按钮
class CustomHeaderView : public QHeaderView {
    Q_OBJECT

public:
    CustomHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QHeaderView(orientation, parent) {
        setSectionsClickable(true);
        setSortIndicatorShown(true);

        // 创建排序按钮
        ascendButton = new QPushButton("", this);
        descendButton = new QPushButton("", this);

        ascendButton->setStyleSheet("background-color:#344659;background-image:url(:/images/uparrow.png);background-repeat:no-repeat;subcontrol-position:center;border:0px");
        descendButton->setStyleSheet("background-color:#344659;background-image:url(:/images/downarrow.png);background-repeat:no-repeat;subcontrol-position:center;border:0px");

        ascendButton->setFixedSize(15, 15);
        descendButton->setFixedSize(15, 15);

        // 隐藏按钮，只有在特定列时才显示
        ascendButton->hide();
        descendButton->hide();

        // 连接按钮的点击信号到槽
        connect(ascendButton, &QPushButton::clicked, this, &CustomHeaderView::sortAscending);
        connect(descendButton, &QPushButton::clicked, this, &CustomHeaderView::sortDescending);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
            // 假设你想要禁用第一列的点击
            int logicalIndex = logicalIndexAt(event->pos());
            if (logicalIndex == 1 || logicalIndex == 2  || logicalIndex == 3) {
                // 忽略第一列的点击事件
                return;
            }
            // 对于其他列，执行默认的点击处理
            QHeaderView::mousePressEvent(event);
        }
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override {
        QHeaderView::paintSection(painter, rect, logicalIndex);

        if (logicalIndex == 1) { // assuming time column is at index 1
            // 在时间列绘制按钮
            int x = rect.right() - 15;
            int y = rect.center().y() - 15;
            ascendButton->move(x, y);
            ascendButton->show();

            descendButton->move(x, y + 15);
            descendButton->show();
        } else {
            ascendButton->hide();
            descendButton->hide();
        }
    }

private slots:
    void sortAscending() {
        emit sortRequested(Qt::AscendingOrder);
    }

    void sortDescending() {
        emit sortRequested(Qt::DescendingOrder);
    }

signals:
    void sortRequested(Qt::SortOrder order);

private:
    QPushButton *ascendButton;
    QPushButton *descendButton;
};

//结合自定义委托类
class AirSpaceCustomDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    AirSpaceCustomDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {
        // 假设 tableView 有 3 列
        m_editableColumns.reserve(4);
        // 设置所有列都可编辑（或根据需要设置）
        m_editableColumns[1] = false;
        m_editableColumns[2] = false;
        m_editableColumns[3] = false;
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
            if (index.column() == 1 || index.column() == 2 || index.column() == 3) {
                // 自定义第1、2、3列的样式
                painter->save();
                if (option.state & QStyle::State_Selected) {
                    // 如果单元格被选中，设置不同的背景色和文本色
                    painter->setPen(QPen(Qt::white)); // 设置边框颜色为白色
//                    painter->drawRect(option.rect.adjusted(0, 0, -1, -1)); // 绘制矩形，表示选中状态
                    painter->drawRoundedRect(option.rect.adjusted(0, 0, -1, -1), 10, 10); // 绘制圆角矩形，圆角半径为10
                } else {
//                    // 如果单元格未选中，使用默认的背景色和文本色
//                    painter->fillRect(option.rect, option.palette.background().color());
//                    painter->setPen(option.palette.foreground().color());
                }
                painter->drawText(option.rect, Qt::AlignCenter, index.data().toString());
                painter->restore();
            } else {
                // 对于其他列，调用基类的 paint 函数进行绘制
                QStyledItemDelegate::paint(painter, option, index);
        }
    }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        if (index.column() == 0) {
            // 对于第1列，不创建编辑器
            return nullptr;
        }
        if (isColumnEditable(index.column()) && m_editableRow == index.row() && index.column() == 1) { // 第三列是QDateTimeEdit
            QDateTimeEdit *datetimeedit = new QDateTimeEdit(parent);
            datetimeedit->setStyleSheet("color: white; bolid: 2px white");
            return datetimeedit;
        }
        if (isColumnEditable(index.column()) && m_editableRow == index.row() && index.column() == 2) { // 第三列是QComboBox
            QComboBox *editor = new QComboBox(parent);
            editor->setStyleSheet("bolid:2px white");
            editor->addItem("Option 1");
            editor->addItem("Option 2");
            // ... 添加其他选项
            return editor;
        }
        if (isColumnEditable(index.column()) && m_editableRow == index.row() && index.column() == 3) { // 第四列是QComboBox
            QComboBox *editor = new QComboBox(parent);
            editor->setStyleSheet("bolid:2px white");
            editor->addItem("Option 1");
            editor->addItem("Option 2");
            // ... 添加其他选项
            return editor;
        }
        // 对于其他列，使用默认编辑器
//        return QStyledItemDelegate::createEditor(parent, option, index);
        //默认为不可编辑
        return nullptr;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
            // 假设你在模型中使用了QString作为存储类型
            // 你可以从模型索引中获取数据，并设置到组合框的当前文本或当前索引
            QString value = index.model()->data(index, Qt::EditRole).toString();
            int indexToSelect = comboBox->findText(value);
            if (indexToSelect >= 0) {
                comboBox->setCurrentIndex(indexToSelect);
            }
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
            // 设置模型数据为组合框的当前文本或当前索引对应的值
            QString value = comboBox->currentText(); // 或者使用 comboBox->currentData().toString();
            model->setData(index, value, Qt::EditRole);
        } else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }

    // 假设你有一个 QList<int> 或 int 来存储可编辑的行号
    void setEditableRows(const int &row) {
        m_editableRow = row;
    }

    void changeEditableStatus(const bool& editable){
            m_editableColumns[1] = editable;
            m_editableColumns[2] = editable;
            m_editableColumns[3] = editable;
    }
private:
    bool isColumnEditable(int column) const {
        // 假设 editableColumns 是一个包含每列是否可编辑的布尔列表
        return m_editableColumns[column];
    }
private:
    int m_editableRow; // 存储可编辑的行号
    QList<bool> m_editableColumns;
};
//QTableview自定义按钮代理类，QTableview中添加按钮需要使用Painter
class QMyTableViewBtnDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QMyTableViewBtnDelegate(QStringList btnNames, QWidget *parent = 0);
    ~QMyTableViewBtnDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        // 检查是否为第五列（索引从0开始）
        if (index.column() == 4) {
            // 对于第五列，不创建编辑器
            return nullptr;
        }
        // 对于其他列，使用默认编辑器
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void editData(const QModelIndex &index);
    void deleteData(const QModelIndex &index);

private:
    QPoint m_mousePoint;  // 鼠标位置
    int m_nType;  // 按钮状态：0-划过 1-按下
    QStringList m_btnNames;
};
//// 自定义委托
//class CustomTimeDelegate : public QStyledItemDelegate {
//public:
//    CustomTimeDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
//        if (index.column() == 1) { // 假设第二列的列索引是 1
//            QDateTimeEdit *datetimeedit = new QDateTimeEdit(parent);
//            datetimeedit->setStyleSheet("color: white; bolid: 2px white");
//            return datetimeedit;
//        }
//        return QStyledItemDelegate::createEditor(parent, option, index);
//    }

//    // 可能还需要实现 setEditorData 和 setModelData 方法来同步数据

//    // ... 其他必要的方法 ...
//};
//class ComboBoxDelegate : public QStyledItemDelegate {
//    Q_OBJECT
//public:
//    ComboBoxDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
//        if (index.column() == 3) { // 第四列是QComboBox
//            QComboBox *editor = new QComboBox(parent);
//            editor->setStyleSheet("bolid:2px white");
//            editor->addItem("Option 1");
//            editor->addItem("Option 2");
//            // ... 添加其他选项
//            return editor;
//        }
//        return QStyledItemDelegate::createEditor(parent, option, index);
//    }

//    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
//        if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
//            // 假设你在模型中使用了QString作为存储类型
//            // 你可以从模型索引中获取数据，并设置到组合框的当前文本或当前索引
//            QString value = index.model()->data(index, Qt::EditRole).toString();
//            int indexToSelect = comboBox->findText(value);
//            if (indexToSelect >= 0) {
//                comboBox->setCurrentIndex(indexToSelect);
//            }
//        } else {
//            QStyledItemDelegate::setEditorData(editor, index);
//        }
//    }

//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
//        if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
//            // 设置模型数据为组合框的当前文本或当前索引对应的值
//            QString value = comboBox->currentText(); // 或者使用 comboBox->currentData().toString();
//            model->setData(index, value, Qt::EditRole);
//        } else {
//            QStyledItemDelegate::setModelData(editor, model, index);
//        }
//    }

//    // 其他必要的方法，如 updateEditorGeometry 可以从基类继承或根据需要重写
//};

namespace Ui {
class AirSpaceInfoWidget;
}

class AirSpaceInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AirSpaceInfoWidget(QWidget *parent = nullptr);
    ~AirSpaceInfoWidget();

private:
//    void setAirSpaceInfoTableText(int row, int col, const QString & text);
//    void	reloadAirSpaceInfoWithButtons();
private slots:
    // 更新列的可编辑状态（例如，在按钮的槽函数中）
    void onEditButtonClicked(const QModelIndex &index);
    //
    void onDeleteButtonClicked(const QModelIndex &index);

    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
private:
    Ui::AirSpaceInfoWidget *ui;
    AirSpaceCustomDelegate *m_Delegate;
    CustomTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QItemSelectionModel *selectionModel;
    int m_currentindexrow;
};

#endif // AIRSPACEINFOWIDGET_H
