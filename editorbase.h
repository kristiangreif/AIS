#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QtSql>
#include <QtWidgets>

class EditorBase : public QWidget
{
    Q_OBJECT

public:
    explicit EditorBase(QWidget *parent = nullptr);

private:
    QWidget *filterHeader;
    QHBoxLayout *filterHeaderLayout;
    QLabel *filterLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterInput;
    QComboBox *filterColumnSelect;

    QWidget *editor;
    QHBoxLayout *editorLayout;

    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *addRowButton;
    QPushButton *deleteRowButton;
    QDialogButtonBox *buttonBox;
};

#endif // EDITORBASE_H
