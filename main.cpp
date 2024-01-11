#include "window.h"
#include <iostream>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>
#include <QtWidgets>

#include "tableeditor.h"

// void initializeStudentsModel(QSqlTableModel *model){
//     model->setTable("Students");
//     model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     model->select();

//     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
//     model->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
//     model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
//     model->setHeaderData(3, Qt::Horizontal, QObject::tr("GPA"));
// }

// void initializeCoursesModel(QSqlTableModel *model){
//     model->setTable("Courses");
//     model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     model->select();

//     model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
//     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
// }

// void initializeEvaluationModel(QSqlRelationalTableModel *model){
//     model->setTable("Evaluation");
//     model->setEditStrategy(QSqlTableModel::OnManualSubmit);

//     model->setRelation(0, QSqlRelation("Students", "id", "first_name"));
//     model->setRelation(1, QSqlRelation("Courses", "id", "name"));

//     model->setHeaderData(0, Qt::Horizontal, QObject::tr("Student"));
//     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Course"));
//     model->setHeaderData(2, Qt::Horizontal, QObject::tr("Grade"));

//     model->select();
// }

// void createRelationalTables()
// {
//     QSqlQuery query;
//     query.exec("create table employee(id int primary key, name varchar(20), city int, country int)");
//     query.exec("insert into employee values(1, 'Espen', 5000, 47)");
//     query.exec("insert into employee values(2, 'Harald', 80000, 49)");
//     query.exec("insert into employee values(3, 'Sam', 100, 1)");

//     query.exec("create table city(id int, name varchar(20))");
//     query.exec("insert into city values(100, 'San Jose')");
//     query.exec("insert into city values(5000, 'Oslo')");
//     query.exec("insert into city values(80000, 'Munich')");

//     query.exec("create table country(id int, name varchar(20))");
//     query.exec("insert into country values(1, 'USA')");
//     query.exec("insert into country values(47, 'Norway')");
//     query.exec("insert into country values(49, 'Germany')");
// }

// std::unique_ptr<QTableView> createRelationalView(QSqlTableModel *model)
// {
//     std::unique_ptr<QTableView> view{new QTableView};
//     view->setModel(model);
//     view->setItemDelegate(new QSqlRelationalDelegate(view.get()));
//     view->resizeColumnsToContents();
//     // view->setWindowTitle(title);

//     return view;
// }

// QTableView *createClassicView(QSqlTableModel *model)
// {
//     QTableView *view{new QTableView};
//     view->setModel(model);
//     view->resizeColumnsToContents();
//     // view->setWindowTitle(title);

//     return view;
// }


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "AIS_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Window w;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Faculty");
    db.setUserName("kristian");
    db.setPassword("?8?57/KG217");

    bool ok = db.open();

    std::cout<<(ok ? "Connection to database successful" : "Cannot connest to database")<<std::endl;

    // QSqlRelationalTableModel evaluationModel;
    // QSqlTableModel studentsModel, coursesModel;

    // initializeStudentsModel(&studentsModel);
    // initializeCoursesModel(&coursesModel);
    // initializeEvaluationModel(&evaluationModel);

    // QWidget *evaluationView = new QWidget();
    // QWidget *studentsView = new QWidget();
    // QWidget *coursesView = new QWidget();



    QHBoxLayout *mainLayout = new QHBoxLayout();

    QTabWidget *tabWidget = new QTabWidget();
    QWidget *studentsPage = new QWidget();
    QWidget *coursesPage = new QWidget();
    QWidget *evaluationPage = new QWidget();

    TableEditor studentsEditor("Students", studentsPage);
    TableEditor coursesEditor("Courses", coursesPage);
    TableEditor evaluationEditor("Evaluation", evaluationPage);

    tabWidget->addTab(studentsPage, "Students");
    tabWidget->addTab(coursesPage, "Courses");
    tabWidget->addTab(evaluationPage, "Evaluation");

    mainLayout->addWidget(tabWidget);
    // mainLayout->addWidget(coursesView);
    // mainLayout->addWidget(evaluationView);
    w.setLayout(mainLayout);

    // QHBoxLayout *studentsPageLayout = new QHBoxLayout();
    // studentsPageLayout->addWidget(studentsView);
    // studentsPage->setLayout(studentsPageLayout);

    // QHBoxLayout *coursesPageLayout = new QHBoxLayout();
    // coursesPageLayout->addWidget(&coursesView);
    // coursesPage->setLayout(coursesPageLayout);

    // QHBoxLayout *evaluationPageLayout = new QHBoxLayout();
    // evaluationPageLayout->addWidget(&evaluationView);
    // evaluationPage->setLayout(evaluationPageLayout);

    // studentsModel.insertRecord(-1, const QSqlRecord &record)

    w.setWindowTitle("AIS");
    w.show();
    return a.exec();
}
