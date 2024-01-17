#include "relationaleditor.h"

#include <QtWidgets>
#include <QtSql>

#include <memory>
#include <algorithm>

#include "sqlrelationalproxydelegate.h"

RelationalEditor::RelationalEditor(const QString &tableName, QStringList header, QList<int> hiddenColumns, QWidget *parent) : QWidget(parent)
{
    model = new QSqlRelationalTableModel(this);

    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setRelation(1, QSqlRelation("Students", "id", "name"));
    model->setRelation(2, QSqlRelation("Courses", "id", "name"));

    for (const auto& i : header){
        model->setHeaderData(header.indexOf(i), Qt::Horizontal, QObject::tr(i.toLocal8Bit().data()));
    }

    model->select();

    proxyModel = new MySortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterRole(Qt::EditRole);

    // QTableView *view{new QTableView};
    view = std::unique_ptr<QTableView> {new QTableView};

    view->setSortingEnabled(true);
    view->setModel(proxyModel);
    view->setItemDelegateForColumn(1, new SqlRelationalProxyDelegate(view.get()));
    view->setItemDelegateForColumn(2, new SqlRelationalProxyDelegate(view.get()));
    view->resizeColumnsToContents();

    for (int i : hiddenColumns) {
        view->hideColumn(i);
    }

    addRowButton = new QPushButton(tr("&Add"));
    deleteRowButton = new QPushButton(tr("&Delete"));
    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    // buttonBox->addButton(refreshButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(addRowButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteRowButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);


    // connect(refreshButton, &QPushButton::clicked, this, &TableEditor::refresh);
    connect(addRowButton, &QPushButton::clicked, this, &RelationalEditor::addRow);
    connect(deleteRowButton, &QPushButton::clicked, this, &RelationalEditor::deleteRow);
    connect(submitButton, &QPushButton::clicked, this, &RelationalEditor::submit);
    connect(revertButton, &QPushButton::clicked,  this, &RelationalEditor::revert);

    //filter header
    filterHeader = new QWidget();
    filterHeaderLayout = new QHBoxLayout();

    filterInput = new QLineEdit;
    filterColumnSelect = new QComboBox;

    filterLabel = new QLabel("Filter:");
    filterLabel->setBuddy(filterInput);
    filterColumnLabel = new QLabel("Category:");
    filterColumnLabel->setBuddy(filterColumnSelect);

    connect(filterColumnSelect, &QComboBox::currentIndexChanged, this, &RelationalEditor::filterSelectSlot);
    connect(filterInput, &QLineEdit::textChanged, this, &RelationalEditor::filterSlot);
    connect(this, &RelationalEditor::filterSignal, proxyModel, &MySortFilterProxyModel::setFilter1);

    QStringList columnNames;
    for (int i=0; i<model->columnCount();i++){
        columnNames << model->headerData(i,Qt::Horizontal).value<QString>();
    }

    filterColumnSelect->addItems(columnNames);

    filterHeaderLayout->setContentsMargins(0,0,0,0);
    filterHeaderLayout->addWidget(filterLabel);
    filterHeaderLayout->addWidget(filterInput);
    filterHeaderLayout->addWidget(filterColumnLabel);
    filterHeaderLayout->addWidget(filterColumnSelect);
    filterHeader->setLayout(filterHeaderLayout);

    editor = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    editorLayout = new QHBoxLayout;

    statistics = new QWidget;
    statisticsLayout = new QHBoxLayout;
    numberOfResults = new QLabel;
    numberOfResults->setText(QString("Number of results: %1").arg(proxyModel->rowCount()));

    connect(proxyModel, &MySortFilterProxyModel::rowAccepted, this, &RelationalEditor::filterFinished);

    statisticsLayout->setContentsMargins(0,0,0,0);
    statisticsLayout->addWidget(numberOfResults);
    statistics->setLayout(statisticsLayout);

    editorLayout->setContentsMargins(0,0,0,0);
    editorLayout->addWidget(view.get());
    editorLayout->addWidget(buttonBox);
    editor->setLayout(editorLayout);

    mainLayout->addWidget(filterHeader);
    mainLayout->addWidget(editor);
    mainLayout->addWidget(statistics);

    parent->setLayout(mainLayout);
}

void RelationalEditor::submit()
{
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            if(!view->isRowHidden(current)){
                prev = current;
                continue;
            }
            view->showRow(current);
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

void RelationalEditor::addRow()
{
    model->insertRow(model->rowCount(QModelIndex()));
}

void RelationalEditor::deleteRow()
{
    // model->removeRow(view->selectionModel()->currentIndex().row());
    QItemSelection selection( view->selectionModel()->selection());
    foreach( const QModelIndex & index, selection.indexes() ) {
        rows.append( index.row() );
    }
    std::sort( rows.begin(), rows.end() );
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            model->removeRows( current, 1 );
            view->hideRow(current);
            prev = current;
        }
    }

}

void RelationalEditor::revert()
{
    model->revertAll();
    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
        int current = rows[i];
        if( current != prev ) {
            if(view->isRowHidden(current)){
                prev = current;
                continue;
            }
            view->showRow(current);
            prev = current;
        }
    }

}

void RelationalEditor::refresh()
{
    // model->setTable("Evaluation");
    model->setRelation(1, QSqlRelation("Students", "id", "name"));
    model->setRelation(2, QSqlRelation("Courses", "id", "name"));
    model->select();
    view->resizeColumnsToContents();
    view->hideColumn(0);
}


void RelationalEditor::filterSlot(const QString& filterText){
    emit filterSignal(filterText, filterColumnSelect->currentIndex());
}

void RelationalEditor::filterSelectSlot(int index){
    emit filterSignal(filterInput->text(), index);
}


void RelationalEditor::filterFinished(const QString& regExp)
{
    int result = 0;
    result = proxyModel->rowCount();

    numberOfResults->setText(QString("Number of results: %1").arg(result));

}
