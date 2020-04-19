#include "transaction_table_model.h"

namespace gui {
TransactionTable::TransactionTable(
    std::vector<finance::Transaction> transactions)
    : transactions_(std::move(transactions)) {
  std::sort(
      transactions_.begin(), transactions_.end(),
      [](const finance::Transaction &lhs, const finance::Transaction &rhs) {
        return lhs.amount < rhs.amount;
      });
}
int TransactionTable::rowCount(const QModelIndex &parent) const {
  std::ignore = parent;
  return transactions_.size();
}
int TransactionTable::columnCount(const QModelIndex &parent) const {
  std::ignore = parent;
  return 4;
}
QVariant TransactionTable::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  if (static_cast<std::size_t>(index.row()) >= transactions_.size())
    return QVariant();
  switch (index.column()) {
  case 0:
    return QString::fromStdString(
        std::to_string(transactions_[index.row()].amount));
    break;
  case 1:
    return QString::fromStdString(
        finance::to_string(transactions_[index.row()].date));
    break;
  case 2:
    return QString::fromStdString(transactions_[index.row()].partner_name);
    break;
  case 3:
    return QString::fromStdString(transactions_[index.row()].reason);
    break;
  default:
    return QVariant();
    break;
  }
  return QString::fromStdString(std::to_string(index.row()));
}
QVariant TransactionTable::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
    return QVariant();
  }
  switch (section) {
  case 0:
    return QString("Amount");
    break;
  case 1:
    return QString("Date");
    break;
  case 2:
    return QString("Partner Name");
    break;
  case 3:
    return QString("Reason");
    break;
  default:
    return QVariant();
    break;
  }
}
} // namespace gui