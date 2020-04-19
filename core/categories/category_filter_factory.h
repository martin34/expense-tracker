#pragma once

#include <istream>
#include <memory>
#include <vector>

#include "core/transaction.h"
#include "i_category_filter.h"

namespace finance {

std::vector<std::unique_ptr<ICategoryFilter>>
CreateCategoryFilters(std::istream &stream);

} // namespace finance