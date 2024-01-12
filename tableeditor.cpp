// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include <iostream>
// #include <stdio.h>
#include <QtWidgets>
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>

#include <memory>
#include <algorithm>

#include "tableeditor.h"


void TableEditor::initializeStudentsModel(QSqlTableModel *model){
    model->setTable("Students");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("GPA"));
}
void TableEditor::initializeCoursesModel(QSqlTableModel *model){
    model->setTable("Courses");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
}
void TableEditor::initializeEvaluationModel(QSqlRelationalTableModel *model){
    model->setTable("Evaluation");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setRelation(1, QSqlRelation("Students", "id", "name"));
    model->setRelation(2, QSqlRelation("Courses", "id", "name"));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Course"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade"));

    model->select();
}
QTableView *TableEditor::createClassicView(QSqlTableModel *model){
    QTableView *view{new QTableView};
    view->setModel(model);
    view->resizeColumnsToContents();
    // view->setWindowTitle(title);

    return view;
}
std::unique_ptr<QTableView> TableEditor::createRelationalView(QSqlTableModel *model){
    std::unique_ptr<QTableView> view{new QTableView};
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view.get()));
    view->resizeColumnsToContents();
    // view->setWindowTitle(title);

    return view;
}

TableEditor::TableEditor(const QString &tableName, QWidget *parent)
    : QWidget(parent)
{
    view = new QTableView;
    evaluationView = std::unique_ptr<QTableView> {new QTableView};

    // model = new QSqlTableModel(this);
    // relationalModel = new QSqlRelationalTableModel(this);
    model = new QSqlRelationalTableModel(this);

    if (tableName == "Students"){
        bool relational = false;
        initializeStudentsModel(model);
        view = createClassicView(model);
        view->hideColumn(3);
    } else if (tableName == "Courses"){
        bool relational = false;
        initializeCoursesModel(model);
        view = createClassicView(model);
    } else if (tableName == "Evaluation"){
        relational = true;
        initializeEvaluationModel(model);
        evaluationView = createRelationalView(model);
        evaluationView->hideColumn(0);
    }

    refreshButton = new QPushButton(tr("&Refresh"));
    addRowButton = new QPushButton(tr("&Add"));
    deleteRowButton = new QPushButton(tr("&Delete"));
    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(refreshButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(addRowButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteRowButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);


    connect(refreshButton, &QPushButton::clicked, this, &TableEditor::refresh);
    connect(addRowButton, &QPushButton::clicked, this, &TableEditor::addRow);
    connect(deleteRowButton, &QPushButton::clicked, this, &TableEditor::deleteRow);
    connect(submitButton, &QPushButton::clicked, this, &TableEditor::submit);
    connect(revertButton, &QPushButton::clicked,  this, &TableEditor::revert);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(relational ? evaluationView.get() : view);

    mainLayout->addWidget(buttonBox);
    parent->setLayout(mainLayout);

}

void TableEditor::submit()
{
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            if(!(relational ? evaluationView->isRowHidden(current) : view->isRowHidden(current))){
                prev = current;
                continue;
            }
            (relational ? evaluationView->showRow(current) : view->showRow(current));
            prev = current;
        }
    }

    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                                 .arg(model->lastError().text()));
    }
}

void TableEditor::addRow()
{
    model->insertRow(model->rowCount(QModelIndex()));
}

void TableEditor::deleteRow()
{
    // model->removeRow(view->selectionModel()->currentIndex().row());
    QItemSelection selection( (relational ? evaluationView->selectionModel()->selection() : view->selectionModel()->selection()) );
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append( index.row() );
    }
    std::sort( rows.begin(), rows.end() );
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            model->removeRows( current, 1 );
            (relational ? evaluationView->hideRow(current) : view->hideRow(current));
            prev = current;
        }
    }

}

void TableEditor::revert()
{
    model->revertAll();
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            if(!(relational ? evaluationView->isRowHidden(current) : view->isRowHidden(current))){
                prev = current;
                continue;
            }
            (relational ? evaluationView->showRow(current) : view->showRow(current));
            prev = current;
        }
    }

}

void TableEditor::refresh()
{
    if(relational) {
        initializeEvaluationModel(model);
        evaluationView->resizeColumnsToContents();
        evaluationView->hideColumn(0);

        return;
    }


    model->select();
}



