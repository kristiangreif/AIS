#include "window.h"
#include "./ui_window.h"


Window::Window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);
}

Window::~Window()
{
    delete ui;
}

void Window::refreshCurrentTab(int index){
    switch (index) {
    case 0:
        studentsEditor->refresh();
        break;
    case 1:
        coursesEditor->refresh();
        break;
    case 2:
        evaluationEditor->refresh();
        break;
    case 3:
        studentsOverview->refresh();
        break;
    case 4:
        coursesOverview->refresh();
        break;
    case 5:
        gradesOverview->refresh();
        break;
    default:
        break;
    }
}

void Window::hideEditorTabs(){
    overviewModeButton->setDefault(true);
    editModeButton->setDefault(false);

    QList<bool> hideIndexes = {0,0,0,1,1,1};

    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->setTabVisible(i, hideIndexes[i]);
    }
}

void Window::hideOverviewTabs(){
    editModeButton->setDefault(true);
    overviewModeButton->setDefault(false);

    QList<bool> hideIndexes = {1,1,1,0,0,0};

    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->setTabVisible(i, hideIndexes[i]);
    }
}

