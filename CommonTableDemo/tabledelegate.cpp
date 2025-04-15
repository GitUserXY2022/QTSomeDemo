#include "tabledelegate.h"

TableDelegate::TableDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

QWidget * TableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    editor->setText(index.model()->data(index, Qt::EditRole).toString());
    return editor;
}

void TableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        lineEdit->setText(index.data(Qt::EditRole).toString());
    } else if (QComboBox *comboBox = qobject_cast<QComboBox*>(editor)) {
        QString currentText = index.data(Qt::EditRole).toString();
        int index = comboBox->findText(currentText);
        comboBox->setCurrentIndex(index);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const {
    QString value = qobject_cast<QLineEdit*>(editor)->text();
    model->setData(index, value, Qt::EditRole);
}

void TableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}

void TableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.row() % 2 == 0) {
        painter->fillRect(option.rect, QColor(83,83,83));
    } else {
        painter->fillRect(option.rect, QColor(66,66,66));
    }
    QStyledItemDelegate::paint(painter, option, index);
}
