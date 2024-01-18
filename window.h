#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
// #include "tableeditor.h"
#include "classiceditor.h"
#include "relationaleditor.h"
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

    // UI elements
    QTabWidget *tabWidget;

    QPushButton *editModeButton;
    QPushButton *overviewModeButton;

    // Editors and overviews
    RelationalEditor *evaluationEditor;
    ClassicEditor *studentsEditor;
    ClassicEditor *coursesEditor;
    Overview *studentsOverview;
    Overview *coursesOverview;
    Overview *gradesOverview;

public slots:
    // Slots for refreshing and hiding corresponding tables
    void refreshCurrentTab(int index);
    void hideOverviewTabs();
    void hideEditorTabs();

private:
    Ui::Window *ui;
};
#endif // WINDOW_H
