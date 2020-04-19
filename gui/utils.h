#pragma once

#include "core/date.h"

#include <QtCore/QDate>

namespace gui {
finance::Date FromQDate(const QDate &date);
}