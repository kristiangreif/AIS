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
#include "mysortfilterproxymodel.h"

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

MySortFilterProxyModel *proxyModel;

QTableView* createView(QSqlQueryModel *model)
{
    QTableView *view = new QTableView;

    proxyModel = new MySortFilterProxyModel(model);
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
    QLineEdit *filter1Input = new QLineEdit;
    QLineEdit *filter2Input = new QLineEdit;
    w.filter1ColumnSelect = new QComboBox;
    w.filter2ColumnSelect = new QComboBox;

    QLabel *filterLabel = new QLabel("Filter:");
    filterLabel->setBuddy(filter1Input);
    QLabel *filter1ColumnLabel = new QLabel("Category:");
    filter1ColumnLabel->setBuddy(w.filter1ColumnSelect);
    QLabel *filter2ColumnLabel = new QLabel("Category:");
    filter2ColumnLabel->setBuddy(w.filter2ColumnSelect);

    QObject::connect(filter1Input, &QLineEdit::textChanged, &w, &Window::filter1Slot);
    QObject::connect(filter2Input, &QLineEdit::textChanged, &w, &Window::filter2Slot);
    QObject::connect(&w, &Window::filter1Signal, proxyModel, &MySortFilterProxyModel::setFilter1);
    QObject::connect(&w, &Window::filter2Signal, proxyModel, &MySortFilterProxyModel::setFilter2);

    QStringList columnNames;
    for (int i=0; i<editableModel.columnCount();i++){
        columnNames << editableModel.headerData(i,Qt::Horizontal).value<QString>();
    }
    w.filter1ColumnSelect->addItems(columnNames);
    w.filter2ColumnSelect->addItems(columnNames);

    filterHeaderLayout->setContentsMargins(0, 0, 0, 0);
    filterHeaderLayout->addWidget(filterLabel);
    filterHeaderLayout->addWidget(filter1Input);
    filterHeaderLayout->addWidget(filter1ColumnLabel);
    filterHeaderLayout->addWidget(w.filter1ColumnSelect);

    filterHeaderLayout->addWidget(filter2Input);
    filterHeaderLayout->addWidget(filter2ColumnLabel);
    filterHeaderLayout->addWidget(w.filter2ColumnSelect);
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
