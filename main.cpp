#include "window.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>

#include <QtWidgets>
#include <QtSql>

#include "relationaleditor.h"
#include "classiceditor.h"
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

    // Main window
    Window w;


    // Database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Faculty");
    db.setUserName("kristian");
    db.setPassword("?8?57/KG217");

    bool ok = db.open();

    qDebug()<<(ok ? "Connection to database successful" : "Cannot connect to database");

    // Prepare Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Add buttons to switch between edit and overview mode
    QWidget *modeSwitch = new QWidget;
    QHBoxLayout *modeSwitchLayout = new QHBoxLayout();

    w.editModeButton = new QPushButton(QObject::tr("&Edit Mode"));
    w.overviewModeButton = new QPushButton(QObject::tr("&Overview"));

    modeSwitchLayout->setContentsMargins(0,10,0,10);
    modeSwitchLayout->addWidget(w.editModeButton);
    modeSwitchLayout->addWidget(w.overviewModeButton);
    modeSwitch->setLayout(modeSwitchLayout);

    // Main Tab widget with all the pages
    w.tabWidget = new QTabWidget();
    QWidget *studentsPage = new QWidget();
    QWidget *coursesPage = new QWidget();
    QWidget *evaluationPage = new QWidget();
    QWidget *studentOverviewPage = new QWidget();
    QWidget *coursesOverviewPage = new QWidget();
    QWidget *gradesOverviewPage = new QWidget();

    // Hide tabs according to the chosen mode
    QWidget::connect(w.editModeButton, &QPushButton::clicked, &w, &Window::hideOverviewTabs);
    QWidget::connect(w.overviewModeButton, &QPushButton::clicked, &w, &Window::hideEditorTabs);


    // Create Editors
    QStringList studentsEditorHeader = {"Student ID", "First Name", "Last Name", "Name"};
    w.studentsEditor = new ClassicEditor("Students", studentsEditorHeader, QList<int> {3}, studentsPage);

    QStringList coursesEditorHeader = {"ID", "Name", "Teacher's First Name", "Teacher's Last Name"};
    w.coursesEditor = new ClassicEditor("Courses", coursesEditorHeader, QList<int> {0}, coursesPage);

    QStringList evaluationEditorHeader = {"ID", "Student's Name", "Course", "Grade"};
    w.evaluationEditor = new RelationalEditor("Evaluation", evaluationEditorHeader, QList<int> {0}, evaluationPage);

    // Create Overviews
    QStringList studentsOverviewHeader = {"Student ID", "First Name", "Last Name", "Course", "Grade"};
    w.studentsOverview = new Overview("SELECT Students.ID, Students.first_name, Students.last_name, Courses.name, Evaluation.grade FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID", studentsOverviewHeader, studentOverviewPage);

    QStringList coursesOverviewHeader = {"Course", "Teacher's First Name", "Teacher's Last Name","Student's First Name", "Student's Last Name", "Grade"};
    w.coursesOverview = new Overview("SELECT Courses.name, Courses.teacher_first_name, Courses.teacher_last_name, Students.first_name, Students.last_name, Evaluation.grade FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID", coursesOverviewHeader, coursesOverviewPage);

    QStringList gradesOverviewHeader = {"Grade", "Student's First Name", "Student's Last Name", "Course"};
    w.gradesOverview = new Overview("SELECT Evaluation.grade, Students.first_name, Students.last_name, Courses.name FROM Evaluation INNER JOIN Students ON Evaluation.student_id = Students.ID INNER JOIN Courses ON Evaluation.course_id = Courses.ID", gradesOverviewHeader, gradesOverviewPage);

    // Refresh database model on tab change
    QObject::connect(w.tabWidget, &QTabWidget::currentChanged, &w, &Window::refreshCurrentTab);

    // Add all the pages to the tab widget
    w.tabWidget->addTab(studentsPage, "Students");
    w.tabWidget->addTab(coursesPage, "Courses");
    w.tabWidget->addTab(evaluationPage, "Evaluation");
    w.tabWidget->addTab(studentOverviewPage, "Student Overview");
    w.tabWidget->addTab(coursesOverviewPage, "Course Overview");
    w.tabWidget->addTab(gradesOverviewPage, "Evaluation Overview");

    // Add tab widget and mode buttons to the main layout
    mainLayout->addWidget(modeSwitch);
    mainLayout->addWidget(w.tabWidget);
    w.setLayout(mainLayout);

    // Set overview as the default mode
    w.hideEditorTabs();
    w.overviewModeButton->setDefault(true);

    w.setWindowTitle("AIS");
    w.show();
    return a.exec();
}
