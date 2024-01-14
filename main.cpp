#include "window.h"

#include <iostream>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QtWidgets>
#include <QtSql>

#include "tableeditor.h"
#include "overview.h"


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

    std::cout<<(ok ? "Connection to database successful" : "Cannot connect to database")<<std::endl;

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QTabWidget *tabWidget = new QTabWidget();
    QWidget *studentsPage = new QWidget();
    QWidget *coursesPage = new QWidget();
    QWidget *evaluationPage = new QWidget();
    QWidget *studentOverviewPage = new QWidget();
    QWidget *coursesOverviewPage = new QWidget();
    QWidget *gradesOverviewPage = new QWidget();

    TableEditor studentsEditor("Students", studentsPage);
    TableEditor coursesEditor("Courses", coursesPage);
    w.evaluationEditor = new TableEditor("Evaluation", evaluationPage);
    QObject::connect(tabWidget, &QTabWidget::currentChanged, &w, &Window::refreshCurrentTab);

    QStringList studentsOverviewHeader = {"Student ID", "QOFirst Name", "Last Name", "Course", "Grade"};
    Overview studentsOverview("SELECT Students.ID, Students.first_name, Students.last_name, Courses.name, Evaluation.grade FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID", studentsOverviewHeader, studentOverviewPage);

    QStringList coursesOverviewHeader = {"Course", "Teacher's First Name", "Teacher's Last Name","Student's First Name", "Student's Last Name", "Grade"};
    Overview coursesOverview("SELECT Courses.name, Courses.teacher_first_name, Courses.teacher_last_name, Students.first_name, Students.last_name, Evaluation.grade FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID", coursesOverviewHeader, coursesOverviewPage);

    QStringList gradesOverviewHeader = {"Grade", "Student's First Name", "Student's Last Name", "Course"};
    Overview gradesOverview("SELECT Evaluation.grade, Students.first_name, Students.last_name, Courses.name FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID", gradesOverviewHeader, gradesOverviewPage);

    tabWidget->addTab(studentsPage, "Students");
    tabWidget->addTab(coursesPage, "Courses");
    tabWidget->addTab(evaluationPage, "Evaluation");
    tabWidget->addTab(studentOverviewPage, "Student Overview");
    tabWidget->addTab(coursesOverviewPage, "Course Overview");
    tabWidget->addTab(gradesOverviewPage, "Evaluation Overview");

    mainLayout->addWidget(tabWidget);
    w.setLayout(mainLayout);

    w.setWindowTitle("AIS");
    w.show();
    return a.exec();
}
