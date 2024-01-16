// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

#include "mysortfilterproxymodel.h"

// QT_BEGIN_NAMESPACE
// class QDialogButtonBox;
// class QPushButton;
// class QSqlTableModel;
// class QSqlRelationalTableModel;
// QT_END_NAMESPACE

//! [0]
class TableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditor(const QString &tableName, QWidget *parent = nullptr);

public slots:
    void refresh();

private slots:
    void submit();
    void addRow();
    void deleteRow();
    void revert();

    void filterSlot(const QString& filterText);
    void filterSelectSlot(int index);

signals:
    void filterSignal(const QString& filterText, int columnIndex);

private:
    QWidget *filterHeader;
    QHBoxLayout *filterHeaderLayout;
    QLabel *filterLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterInput;
    QComboBox *filterColumnSelect;

    QWidget *editor;
    QHBoxLayout *editorLayout;

    // QPushButton *refreshButton;
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *addRowButton;
    QPushButton *deleteRowButton;
    QDialogButtonBox *buttonBox;
    // QSqlTableModel *model;
    QSqlRelationalTableModel *model;
    MySortFilterProxyModel *proxyModel;
    // QSqlRelationalTableModel *relationalModel;
    QTableView *view;
    std::unique_ptr<QTableView> evaluationView;
    bool relational = false;
    QList<int> rows;

    void initializeStudentsModel(QSqlTableModel *model);
    void initializeCoursesModel(QSqlTableModel *model);
    void initializeEvaluationModel(QSqlRelationalTableModel *model);
    QTableView *createClassicView(QSqlTableModel *model);
    std::unique_ptr<QTableView> createRelationalView(QSqlTableModel *model);
};
//! [0]

#endif
