#include "sqlrelationalproxydelegate.h"

#include <QSortFilterProxyModel>
#include <QDebug>

SqlRelationalProxyDelegate::SqlRelationalProxyDelegate(QObject* parent)
    : QSqlRelationalDelegate(parent)
{}

QWidget *SqlRelationalProxyDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
    //    const QSortFilterProxyModel* proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    //    Q_ASSERT(proxyModel);
    //    const QSqlRelationalTableModel* sqlModel = qobject_cast<const QSqlRelationalTableModel*>(proxyModel->sourceModel());
    //    Q_ASSERT(sqlModel);
    //    QSqlTableModel* childModel = sqlModel->relationModel(index.column());
    //    Q_ASSERT(childModel);
    //    while(childModel->canFetchMore()) {
    //        childModel->fetchMore();
    //    }
    //    QModelIndex baseIndex = proxyModel->mapToSource(index);

    //    return QSqlRelationalDelegate::createEditor(parent, option, baseIndex);

    const QSortFilterProxyModel* proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    Q_ASSERT(proxyModel);
    QModelIndex baseIndex = proxyModel->mapToSource(index);

    QComboBox* combo = qobject_cast<QComboBox*>(QSqlRelationalDelegate::createEditor(parent, option, baseIndex));
    while (combo->model()->canFetchMore(QModelIndex())) {
        combo->model()->fetchMore(QModelIndex());
    }

    return combo;
}

void SqlRelationalProxyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QSortFilterProxyModel* proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    Q_ASSERT(proxyModel);
    QModelIndex baseIndex = proxyModel->mapToSource(index);

    QSqlRelationalDelegate::setEditorData(editor, baseIndex);
}

void SqlRelationalProxyDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
    QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(model);
    Q_ASSERT(proxyModel);
    QSqlRelationalTableModel* baseModel = qobject_cast<QSqlRelationalTableModel*>(proxyModel->sourceModel());
    Q_ASSERT(baseModel);
    QModelIndex baseIndex = proxyModel->mapToSource(index);

    QSqlRelationalDelegate::setModelData(editor, baseModel, baseIndex);
}
