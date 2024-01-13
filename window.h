#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtWidgets>

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

    QComboBox *filter1ColumnSelect;
    QComboBox *filter2ColumnSelect;

public slots:
    void filter1Slot(const QString& filterText);
    void filter2Slot(const QString& filterText);
signals:
    void filter1Signal(const QString& filterText, int columnIndex);
    void filter2Signal(const QString& filterText, int columnIndex);

private:
    Ui::Window *ui;
};
#endif // WINDOW_H
