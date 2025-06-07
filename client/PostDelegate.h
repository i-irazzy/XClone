#ifndef POSTDELEGATE_H
#define POSTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class PostDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit PostDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.data(Qt::UserRole).toString() == "Удалить пост") {
            painter->drawText(option.rect, Qt::AlignCenter, "Удалить пост");
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

#endif // POSTDELEGATE_H
