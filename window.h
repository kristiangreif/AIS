#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "tableeditor.h"

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

    TableEditor *evaluationEditor;

public slots:
    void refreshCurrentTab(int index);

private:
    Ui::Window *ui;
};
#endif // WINDOW_H
