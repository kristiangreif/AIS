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
    void refresh();

private slots:
    void filter1Slot(const QString& filterText);
    void filter2Slot(const QString& filterText);
    void filter1SelectSlot(int index);
    void filter2SelectSlot(int index);

signals:
    void filter1Signal(const QString& filterText, int columnIndex);
    void filter2Signal(const QString& filterText, int columnIndex);

private:
    QString modelQuery;
    QSqlQueryModel *model;
    MySortFilterProxyModel *proxyModel;
    QTableView *view;

    QWidget *filterHeader;
    QHBoxLayout *filterHeaderLayout;
    QLabel *filterLabel;
    QLabel *filter1ColumnLabel;
    QLabel *filter2ColumnLabel;
    QLineEdit *filter1Input;
    QLineEdit *filter2Input;
    QComboBox *filter1ColumnSelect;
    QComboBox *filter2ColumnSelect;
    QWidget *statistics;

};

#endif // OVERVIEW_H
