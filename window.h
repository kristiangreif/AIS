#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "tableeditor.h"
#include "overview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Window;
}
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

    QTabWidget *tabWidget;

    QPushButton *editModeButton;
    QPushButton *overviewModeButton;

    TableEditor *evaluationEditor;
    TableEditor *studentsEditor;
    TableEditor *coursesEditor;
    Overview *studentsOverview;
    Overview *coursesOverview;
    Overview *gradesOverview;

public slots:
    void refreshCurrentTab(int index);
    void hideOverviewTabs();
    void hideEditorTabs();

private:
    Ui::Window *ui;
};
#endif // WINDOW_H
