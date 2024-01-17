#ifndef CLASSICEDITOR_H
#define CLASSICEDITOR_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

#include "mysortfilterproxymodel.h"

class ClassicEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ClassicEditor(const QString &tableName, QStringList header, QList<int> hiddenColumns, QWidget *parent = nullptr);

public slots:
    void refresh();

private slots:
    void submit();
    void addRow();
    void deleteRow();
    void revert();

    void filterSlot(const QString& filterText);
    void filterSelectSlot(int index);
    void filterFinished(const QString& regExp);

signals:
    void filterSignal(const QString& filterText, int columnIndex);

private:
    QSqlTableModel *model;
    MySortFilterProxyModel *proxyModel;
    QTableView *view;

    QWidget *filterHeader;
    QHBoxLayout *filterHeaderLayout;
    QLabel *filterLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterInput;
    QComboBox *filterColumnSelect;

    QWidget *editor;
    QHBoxLayout *editorLayout;

    QWidget *statistics;
    QHBoxLayout *statisticsLayout;
    QLabel *numberOfResults;

    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *addRowButton;
    QPushButton *deleteRowButton;
    QDialogButtonBox *buttonBox;

    QList<int> rows;


};

#endif // CLASSICEDITOR_H
