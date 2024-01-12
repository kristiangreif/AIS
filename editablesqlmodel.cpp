// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtSql>

#include "editablesqlmodel.h"

EditableSqlModel::EditableSqlModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

//! [0]
Qt::ItemFlags EditableSqlModel::flags(
    const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1 || index.column() == 2)
        flags |= Qt::ItemIsEditable;
    return flags;
}
//! [0]

//! [1]
bool EditableSqlModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() < 1 || index.column() > 2)
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt();

    clear();

    bool ok;
    if (index.column() == 1) {
        ok = setFirstName(id, value.toString());
    } else {
        ok = setLastName(id, value.toString());
    }
    refresh();
    return ok;
}
//! [1]

void EditableSqlModel::refresh()
{
    setQuery("SELECT Students.ID, Students.first_name, Students.last_name, Courses.name, Evaluation.grade, Students.GPA FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID");
    setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
    setHeaderData(3, Qt::Horizontal, QObject::tr("Course Name"));
    setHeaderData(4, Qt::Horizontal, QObject::tr("Grade"));
    setHeaderData(5, Qt::Horizontal, QObject::tr("GPA"));
}

//! [2]
bool EditableSqlModel::setFirstName(int personId, const QString &firstName)
{
    QSqlQuery query;
    query.prepare("update Students set first_name = ? where id = ?");
    query.addBindValue(firstName);
    query.addBindValue(personId);
    return query.exec();
}
//! [2]

bool EditableSqlModel::setLastName(int personId, const QString &lastName)
{
    QSqlQuery query;
    query.prepare("update Students set last_name = ? where id = ?");
    query.addBindValue(lastName);
    query.addBindValue(personId);
    return query.exec();
}
