/*Table editor for QSqlRelationalTableModel,
with the ability to add, delete, modify, sort and filter rows in a single relational database table.
(displaying values from related tables)*/

#ifndef RELATIONALEDITOR_H
#define RELATIONALEDITOR_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

#include "mysortfilterproxymodel.h"

class RelationalEditor : public QWidget
{
    Q_OBJECT

public:
    explicit RelationalEditor(const QString &tableName, QStringList header, QList<int> hiddenColumns, QWidget *parent = nullptr);

public slots:
    // Refresh Model
    void refresh();

private slots:
    // Button slots
    void submit();
    void addRow();
    void deleteRow();
    void revert();

    // Custom filter slots
    void filterSlot(const QString& filterText);
    void filterSelectSlot(int index);
    void filterFinished(const QString& regExp);

signals:
    // Custom filter signal for "mysortfilterproxymodel"
    void filterSignal(const QString& filterText, int columnIndex);

private:
    // Relational Model/View variables
    QSqlRelationalTableModel *model;
    MySortFilterProxyModel *proxyModel;
    std::unique_ptr<QTableView> view;


    // Filter UI elements
    QWidget *filterHeader;
    QHBoxLayout *filterHeaderLayout;
    QLabel *filterLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterInput;
    QComboBox *filterColumnSelect;

    // Main editor
    QWidget *editor;
    QHBoxLayout *editorLayout;

    // Statistics widget
    QWidget *statistics;
    QHBoxLayout *statisticsLayout;
    QLabel *numberOfResults;

    // Editor buttons
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *addRowButton;
    QPushButton *deleteRowButton;
    QDialogButtonBox *buttonBox;

    QList<int> rows;
};

#endif // RELATIONALEDITOR_H
