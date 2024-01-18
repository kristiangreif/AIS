/*Non-editable model/view for QSqlQueryModel,
with the ability sort and 2x filter rows of a query.*/

#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QSqlQueryModel>
#include <QtWidgets>
#include "mysortfilterproxymodel.h"

class Overview : public QWidget
{
    Q_OBJECT

public:
    explicit Overview(const QString& query, QStringList header, QWidget *parent = nullptr);

public slots:
    // Refresh model
    void refresh();

private slots:
    // Custom filter slots
    void filter1Slot(const QString& filterText);
    void filter2Slot(const QString& filterText);
    void filter1SelectSlot(int index);
    void filter2SelectSlot(int index);
    void filterFinished(const QString& regExp);

signals:
    // Custom filter signals for "mysortfilterproxymodel"
    void filter1Signal(const QString& filterText, int columnIndex);
    void filter2Signal(const QString& filterText, int columnIndex);

private:
    // Model/View variables
    QString modelQuery;
    QSqlQueryModel *model;
    MySortFilterProxyModel *proxyModel;
    QTableView *view;

    // Filter UI elements
    QWidget *filterHeader;
    QHBoxLayout *filterHeaderLayout;
    QLabel *filterLabel;
    QLabel *filter1ColumnLabel;
    QLabel *filter2ColumnLabel;
    QLineEdit *filter1Input;
    QLineEdit *filter2Input;
    QComboBox *filter1ColumnSelect;
    QComboBox *filter2ColumnSelect;

    // Statistics widget
    QWidget *statistics;
    QHBoxLayout *statisticsLayout;
    QLabel *numberOfResults;

};

#endif // OVERVIEW_H
