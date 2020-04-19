#pragma once

#include <QtCore/QAbstractTableModel>

#include "tracker/tracker.h"

namespace gui {
class TransactionTable : public QAbstractTableModel {
public:
  TransactionTable(std::vector<finance::Transaction> transactions);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  virtual ~TransactionTable() = default;

private:
  std::vector<finance::Transaction> transactions_;
};

} // namespace gui