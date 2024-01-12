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
#include "editablesqlmodel.h"

void initializeModel(QSqlQueryModel *model)
{
    model->setQuery("SELECT Students.ID, Students.first_name, Students.last_name, Courses.name, Evaluation.grade, Students.GPA FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Course Name"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Grade"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("GPA"));
}

QTableView* createView(QSqlQueryModel *model)
{
    QTableView *view = new QTableView;

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    view->setSortingEnabled(true);

    view->setModel(proxyModel);

    // static int offset = 0;

    // view->setWindowTitle(title);
    // view->move(100 + offset, 100 + offset);
    // offset += 20;
    // view->show();

    return view;
}

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

    QVBoxLayout *overviewLayout = new QVBoxLayout();
    QWidget *studentOverviewPage = new QWidget();

    EditableSqlModel editableModel(studentOverviewPage);
    initializeModel(&editableModel);
    QTableView *studentOverview = new QTableView;
    studentOverview = createView(&editableModel);

    QWidget *filterHeader = new QWidget();
    QHBoxLayout *filterHeaderLayout = new QHBoxLayout();
    QLineEdit *filterInput = new QLineEdit;
    QComboBox *columnSelect = new QComboBox;
    QPushButton *filterButton = new QPushButton;

    filterHeaderLayout->addWidget(filterInput);
    filterHeaderLayout->addWidget(columnSelect);
    filterHeaderLayout->addWidget(filterButton);
    filterHeader->setLayout(filterHeaderLayout);

    overviewLayout->addWidget(filterHeader);
    overviewLayout->addWidget(studentOverview);
    studentOverviewPage->setLayout(overviewLayout);

    tabWidget->addTab(studentsPage, "Students");
    tabWidget->addTab(coursesPage, "Courses");
    tabWidget->addTab(evaluationPage, "Evaluation");
    tabWidget->addTab(studentOverviewPage, "Student Overview");

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
