#pragma once

#include <QtWidgets/QDialog>

#include "transaction_table_model.h"

namespace gui {

class TransactionDetailsDialog : public QDialog {
public:
  TransactionDetailsDialog(TransactionTable *table);

  virtual ~TransactionDetailsDialog() { delete transaction_table_; }

private:
  QLayout *layout{nullptr};
  TransactionTable *transaction_table_{nullptr};
};

} // namespace gui