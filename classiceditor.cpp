#include <memory>
#include <algorithm>

#include "classiceditor.h"

ClassicEditor::ClassicEditor(const QString &tableName, QStringList header, QList<int> hiddenColumns, QWidget *parent) : QWidget(parent)
{
    model = new QSqlTableModel(this);

    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    for (const auto& i : header){
        model->setHeaderData(header.indexOf(i), Qt::Horizontal, QObject::tr(i.toLocal8Bit().data()));
    }

    proxyModel = new MySortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterRole(Qt::EditRole);

    // QTableView *view{new QTableView};
    view = new QTableView;

    view->setSortingEnabled(true);
    view->setModel(proxyModel);
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
    connect(addRowButton, &QPushButton::clicked, this, &ClassicEditor::addRow);
    connect(deleteRowButton, &QPushButton::clicked, this, &ClassicEditor::deleteRow);
    connect(submitButton, &QPushButton::clicked, this, &ClassicEditor::submit);
    connect(revertButton, &QPushButton::clicked,  this, &ClassicEditor::revert);

    //filter header
    filterHeader = new QWidget();
    filterHeaderLayout = new QHBoxLayout();

    filterInput = new QLineEdit;
    filterColumnSelect = new QComboBox;

    filterLabel = new QLabel("Filter:");
    filterLabel->setBuddy(filterInput);
    filterColumnLabel = new QLabel("Category:");
    filterColumnLabel->setBuddy(filterColumnSelect);

    connect(filterColumnSelect, &QComboBox::currentIndexChanged, this, &ClassicEditor::filterSelectSlot);
    connect(filterInput, &QLineEdit::textChanged, this, &ClassicEditor::filterSlot);
    connect(this, &ClassicEditor::filterSignal, proxyModel, &MySortFilterProxyModel::setFilter1);

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

    connect(proxyModel, &MySortFilterProxyModel::rowAccepted, this, &ClassicEditor::filterFinished);

    statisticsLayout->setContentsMargins(0,0,0,0);
    statisticsLayout->addWidget(numberOfResults);
    statistics->setLayout(statisticsLayout);

    editorLayout->setContentsMargins(0,0,0,0);
    editorLayout->addWidget(view);
    editorLayout->addWidget(buttonBox);
    editor->setLayout(editorLayout);

    mainLayout->addWidget(filterHeader);
    mainLayout->addWidget(editor);
    mainLayout->addWidget(statistics);

    parent->setLayout(mainLayout);
}

void ClassicEditor::submit()
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

void ClassicEditor::addRow()
{
    model->insertRow(model->rowCount(QModelIndex()));
}

void ClassicEditor::deleteRow()
{
    // model->removeRow(view->selectionModel()->currentIndex().row());
    QItemSelection selection(view->selectionModel()->selection());
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

void ClassicEditor::revert()
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

void ClassicEditor::refresh()
{
    model->select();
    view->resizeColumnsToContents();
}


void ClassicEditor::filterSlot(const QString& filterText){
    emit filterSignal(filterText, filterColumnSelect->currentIndex());
}

void ClassicEditor::filterSelectSlot(int index){
    emit filterSignal(filterInput->text(), index);
}


void ClassicEditor::filterFinished(const QString& regExp)
{
    int result = 0;
    result = proxyModel->rowCount();

    numberOfResults->setText(QString("Number of results: %1").arg(result));

}
