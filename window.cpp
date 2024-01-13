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

void Window::filter1Slot(const QString& filterText){
    emit filter1Signal(filterText, filter1ColumnSelect->currentIndex());
}

void Window::filter2Slot(const QString& filterText){
    emit filter2Signal(filterText, filter2ColumnSelect->currentIndex());
}
