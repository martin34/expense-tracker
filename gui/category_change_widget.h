#pragma once

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QtWidgets>

#include "category_string_completer.h"
#include "tracker/tracker.h"

namespace gui {

class TransactionTableNew : public QAbstractTableModel {
public:
  TransactionTableNew(finance::Tracker &tracker);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  Qt::ItemFlags flags(const QModelIndex &index) const override {
    std::ignore = index;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  }
  finance::Transactions::const_iterator GetCurrent() { return current_; }
  void Next();
  virtual ~TransactionTableNew() = default;

private:
  finance::Tracker &tracker_;
  std::vector<finance::Transactions::const_iterator> elements_;
  finance::Transactions::const_iterator current_;
};

class ComboBox : public QComboBox {
  Q_OBJECT
public:
  ComboBox() {
    for (const auto v : finance::Categories) {
      addItem(QString::fromStdString(finance::to_string(v)));
    }
    setCurrentIndex(static_cast<int>(finance::Transaction::Unknown));
  }
  virtual void keyPressEvent(QKeyEvent *e) override {
    std::string s = e->text().toStdString();
    if (s.size() == 1) {
      if (isalnum(s[0]) || (e->key() == Qt::Key_Backspace)) {
        completer_.UserInput(e->text().toStdString()[0]);
      }
    }
    if (e->key() == Qt::Key_Escape) {
      Reset();
    }
    auto completed = completer_.Complete();
    if (completed) {
      setCurrentIndex(static_cast<int>(completed.value()));
    } else {
      setCurrentIndex(static_cast<int>(finance::Transaction::Unknown));
    }
    if ((e->key() == Qt::Key::Key_Return) || (e->key() == Qt::Key::Key_Enter)) {
      emit Confirmed(currentText());
    }
  }
  void Reset() {
    completer_.Reset();
    setCurrentIndex(static_cast<int>(finance::Transaction::Unknown));
  }
  virtual ~ComboBox() = default;

signals:
  void Confirmed(const QString &input);

private:
  CategoryStringCompleter completer_;
};

class CategoryChangeWidget : public QWidget {
  Q_OBJECT
public:
  CategoryChangeWidget(QBoxLayout *parent, finance::Tracker &tracker);

private slots:
  void CategoryChangedTo(const QString &category);

private:
  QBoxLayout *parent_{nullptr};
  finance::Tracker &tracker_;
  QTableView table_view_;
  TransactionTableNew model_;
  ComboBox drop_down_;
};

} // namespace gui
