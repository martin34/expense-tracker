#include "transaction_table_view.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLayout>
#include <QtWidgets/QTableView>

namespace gui {
TransactionDetailsDialog::TransactionDetailsDialog(TransactionTable *table)
    : transaction_table_(table) {
  setWindowTitle("Transaction Details");
  setWindowFlags(Qt::Window);
  layout = new QVBoxLayout();

  setLayout(layout);

  QTableView *table_view = new QTableView();
  table_view->setModel(transaction_table_);
  layout->addWidget(table_view);
  table_view->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
}
} // namespace gui