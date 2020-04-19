#include "utils.h"

namespace gui {

finance::Date FromQDate(const QDate &date) {
  return finance::Date(date.year(), date.month(), date.day());
}
} // namespace gui