/*Basic table editor for QSqlTableModel,
with the ability to add, delete, modify, sort and filter rows in a single database table.*/

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
    // Refresh model
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
    // Model/View variables
    QSqlTableModel *model;
    MySortFilterProxyModel *proxyModel;
    QTableView *view;

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

#endif // CLASSICEDITOR_H
