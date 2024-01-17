#include <QtWidgets>
#include <QDebug>
#include "overview.h"

Overview::Overview(const QString& query, QStringList header, QWidget *parent) : QWidget(parent){
    modelQuery = query;

    model = new QSqlQueryModel;
    proxyModel = new MySortFilterProxyModel;

    // "SELECT Students.ID, Students.first_name, Students.last_name, Courses.name, Evaluation.grade, Students.GPA FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID GROUP BY Students.ID, Courses.ID"
    model->setQuery(query);

    for (const auto& i : header){
        model->setHeaderData(header.indexOf(i), Qt::Horizontal, QObject::tr(i.toLocal8Bit().data()));
    }

    proxyModel = new MySortFilterProxyModel(model);
    proxyModel->setSourceModel(model);

    view = new QTableView;
    view->setSortingEnabled(true);
    view->setModel(proxyModel);
    view->resizeColumnsToContents();

    QVBoxLayout *overviewLayout = new QVBoxLayout();

    filterHeader = new QWidget();
    filterHeaderLayout = new QHBoxLayout();

    filter1Input = new QLineEdit;
    filter2Input = new QLineEdit;
    filter1ColumnSelect = new QComboBox;
    filter2ColumnSelect = new QComboBox;

    filterLabel = new QLabel("Filter:");
    filterLabel->setBuddy(filter1Input);
    filter1ColumnLabel = new QLabel("Category:");
    filter1ColumnLabel->setBuddy(filter1ColumnSelect);
    filter2ColumnLabel = new QLabel("Category:");
    filter2ColumnLabel->setBuddy(filter2ColumnSelect);

    connect(filter1ColumnSelect, &QComboBox::currentIndexChanged, this, &Overview::filter1SelectSlot);
    connect(filter2ColumnSelect, &QComboBox::currentIndexChanged, this, &Overview::filter2SelectSlot);
    connect(filter1Input, &QLineEdit::textChanged, this, &Overview::filter1Slot);
    connect(filter2Input, &QLineEdit::textChanged, this, &Overview::filter2Slot);
    connect(this, &Overview::filter1Signal, proxyModel, &MySortFilterProxyModel::setFilter1);
    connect(this, &Overview::filter2Signal, proxyModel, &MySortFilterProxyModel::setFilter2);

    // QStringList columnNames;
    // for (int i=0; i<model.columnCount();i++){
    //     columnNames << model.headerData(i,Qt::Horizontal).value<QString>();
    // }

    filter1ColumnSelect->addItems(header);
    filter2ColumnSelect->addItems(header);

    filterHeaderLayout->setContentsMargins(0, 0, 0, 0);
    filterHeaderLayout->addWidget(filterLabel);
    filterHeaderLayout->addWidget(filter1Input);
    filterHeaderLayout->addWidget(filter1ColumnLabel);
    filterHeaderLayout->addWidget(filter1ColumnSelect);

    filterHeaderLayout->addWidget(filter2Input);
    filterHeaderLayout->addWidget(filter2ColumnLabel);
    filterHeaderLayout->addWidget(filter2ColumnSelect);
    filterHeader->setLayout(filterHeaderLayout);

    // refreshButton = new QPushButton(tr("&Refresh"));
    // connect(refreshButton, &QPushButton::clicked, this, &Overview::refresh);

    statistics = new QWidget;
    statisticsLayout = new QHBoxLayout;
    numberOfResults = new QLabel;
    numberOfResults->setText(QString("Number of results: %1").arg(proxyModel->rowCount()));

    connect(proxyModel, &MySortFilterProxyModel::rowAccepted, this, &Overview::filterFinished);

    statisticsLayout->addWidget(numberOfResults);
    statistics->setLayout(statisticsLayout);


    overviewLayout->addWidget(filterHeader);
    overviewLayout->addWidget(view);
    overviewLayout->addWidget(statistics);

    parent->setLayout(overviewLayout);
}

void Overview::filter1Slot(const QString& filterText){
    emit filter1Signal(filterText, filter1ColumnSelect->currentIndex());
}

void Overview::filter2Slot(const QString& filterText){
    emit filter2Signal(filterText, filter2ColumnSelect->currentIndex());
}

void Overview::filter1SelectSlot(int index){
    emit filter1Signal(filter1Input->text(), index);
}

void Overview::filter2SelectSlot(int index){
    emit filter2Signal(filter2Input->text(), index);
}

void Overview::refresh(){
    model->setQuery(modelQuery);
}

void Overview::filterFinished(const QString& regExp)
{
    int result = 0;

    if(model->headerData(0, Qt::Horizontal) != "Student ID" || regExp == ""){
        result = proxyModel->rowCount();
    } else{
        QStringList keyColumnValues = {};
        for (int i = 0; i < proxyModel->rowCount(); i++) {
            keyColumnValues << proxyModel->data(proxyModel->index(i, 0)).toString();
        }
        keyColumnValues.removeDuplicates();
        result = keyColumnValues.size();
    }

    numberOfResults->setText(QString("Number of results: %1").arg(result));

}
