#include "category_change_widget.h"

namespace gui {

CategoryChangeWidget::CategoryChangeWidget(QBoxLayout *parent,
                                           finance::Tracker &tracker)
    : parent_(parent), tracker_(tracker), model_(TransactionTableNew{tracker}) {
  parent_->addWidget(&table_view_);
  table_view_.setModel(&model_);

  // Improve completion with:
  // https://doc.qt.io/qt-5/qtwidgets-tools-customcompleter-example.html
  QModelIndex index = model_.index(model_.rowCount() - 1, 3);
  table_view_.setIndexWidget(index, &drop_down_);
  table_view_.horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);

  connect(&drop_down_, &ComboBox::Confirmed, this,
          &CategoryChangeWidget::CategoryChangedTo);
  drop_down_.setFocus();
  drop_down_.grabKeyboard();
}

void CategoryChangeWidget::CategoryChangedTo(const QString &category) {
  qDebug() << "New text in spin box: " << category;
  tracker_.GetTransactions().UpdateCategory(
      model_.GetCurrent(), finance::ParseTypeFrom(category.toStdString()));
  model_.Next();
  drop_down_.Reset();
}

// =========================

TransactionTableNew::TransactionTableNew(finance::Tracker &tracker)
    : tracker_{tracker} {
  Next();
}
void TransactionTableNew::Next() {
  finance::Transactions &t = tracker_.GetTransactions();
  auto begin = t.cbegin();
  auto end = t.cend();
  begin = std::find_if(begin, end, [](const auto &t) {
    return t.category == finance::Transaction::Unknown;
  });
  current_ = begin;
  if (begin != t.cend()) {
    elements_.clear();
    for (; (begin != t.cbegin()) && (elements_.size() < 10); --begin) {
      elements_.insert(elements_.begin(), begin);
    }
    qDebug() << "Current: " << QString::fromStdString(current_->partner_name);
    emit dataChanged(index(0, 0), index(rowCount() - 1, 4));
  }
}
int TransactionTableNew::rowCount(const QModelIndex &parent) const {
  std::ignore = parent;
  return elements_.size();
}
int TransactionTableNew::columnCount(const QModelIndex &parent) const {
  std::ignore = parent;
  return 5;
}
QVariant TransactionTableNew::data(const QModelIndex &index, int role) const {

  // Find next element with Unknown category
  // Insert elements before and after to table
  // OnUpdate change category and update table

  if (role != Qt::DisplayRole)
    return QVariant();
  if (static_cast<std::size_t>(index.row()) >= elements_.size())
    return QVariant();
  switch (index.column()) {
  case 0:
    return QString::fromStdString(
        std::to_string(elements_.at(index.row())->amount));
    break;
  case 1:
    return QString::fromStdString(
        finance::to_string(elements_.at(index.row())->date));
    break;
  case 2:
    return QString::fromStdString(elements_.at(index.row())->partner_name);
    break;
  case 3:
    return QString::fromStdString(
        finance::to_string(elements_.at(index.row())->category));
    break;
  case 4:
    return QString::fromStdString(elements_.at(index.row())->reason);
    break;
  default:
    return QVariant();
    break;
  }
  return QString::fromStdString(std::to_string(index.row()));
}
QVariant TransactionTableNew::headerData(int section,
                                         Qt::Orientation orientation,
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
    return QString("Category");
    break;
  case 4:
    return QString("Reason");
    break;
  default:
    return QVariant();
    break;
  }
}
} // namespace gui