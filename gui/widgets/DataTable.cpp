#include "DataTable.h"

#include <QHeaderView>
#include <QLineEdit>
#include <QTableWidget>
#include <QVBoxLayout>

DataTable::DataTable(QWidget* parent) : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    m_search = new QLineEdit;
    m_search->setObjectName("searchBox");
    m_search->setPlaceholderText(QString::fromUtf8("\xF0\x9F\x94\x8D  Search..."));
    m_search->setClearButtonEnabled(true);
    connect(m_search, &QLineEdit::textChanged, this, &DataTable::onSearch);
    layout->addWidget(m_search);

    m_table = new QTableWidget;
    m_table->setObjectName("dataTable");
    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSortingEnabled(false);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_table->setShowGrid(false);
    layout->addWidget(m_table, 1);
}

void DataTable::setData(const QStringList& headers, const QVector<QStringList>& rows)
{
    m_headers = headers;
    m_allRows = rows;

    m_table->setSortingEnabled(false);
    m_table->clear();
    m_table->setColumnCount(headers.size());
    m_table->setHorizontalHeaderLabels(headers);
    m_table->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        for (int c = 0; c < headers.size() && c < rows[r].size(); ++c) {
            auto* item = new QTableWidgetItem(rows[r][c]);
            m_table->setItem(r, c, item);
        }
    }
    m_search->clear();
}

void DataTable::setCellWidget(int row, int col, QWidget* w)
{
    m_table->setCellWidget(row, col, w);
}

void DataTable::onSearch(const QString& text)
{
    const QString needle = text.trimmed().toLower();
    for (int r = 0; r < m_table->rowCount(); ++r) {
        bool match = needle.isEmpty();
        if (!match) {
            for (int c = 0; c < m_table->columnCount(); ++c) {
                auto* item = m_table->item(r, c);
                if (item && item->text().toLower().contains(needle)) {
                    match = true;
                    break;
                }
            }
        }
        m_table->setRowHidden(r, !match);
    }
}
