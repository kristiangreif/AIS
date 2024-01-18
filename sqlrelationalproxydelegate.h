// Custom item delegate for a proxied relational table

#include <QSqlRelationalDelegate>

class SqlRelationalProxyDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    SqlRelationalProxyDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
};
