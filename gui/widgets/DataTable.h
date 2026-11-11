#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QStringList>
#include <QVector>

class QLineEdit;
class QTableWidget;

class DataTable : public QWidget {
    Q_OBJECT
public:
    explicit DataTable(QWidget* parent = nullptr);

    void setData(const QStringList& headers, const QVector<QStringList>& rows);
    void setCellWidget(int row, int col, QWidget* w);
    QTableWidget* table() const { return m_table; }

private slots:
    void onSearch(const QString& text);

private:
    QLineEdit*    m_search = nullptr;
    QTableWidget* m_table  = nullptr;
    QVector<QStringList> m_allRows;   // cache for filtering
    QStringList         m_headers;
};

#endif // DATATABLE_H

