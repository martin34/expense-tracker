#include "category_filter_factory.h"

#include <regex>

#include "core/stream/utils.h"
#include "iban_filter.h"
#include "partner_name_filter.h"
#include "reason_filter.h"
#include "transactiontype_filter.h"

namespace finance {

void ValidateHeader(const std::string &line) {
  if (line != "Category;FilterType;Configuration") {
    throw std::domain_error("Filter config not valid (header)");
  }
}

std::vector<std::unique_ptr<ICategoryFilter>>
CreateCategoryFilters(std::istream &stream) {
  std::string line{};
  getline(stream, line);
  ValidateHeader(line);

  std::vector<std::unique_ptr<ICategoryFilter>> category_filters;
  while (getline(stream, line)) {
    std::stringstream line_stream{line};
    std::string word;
    std::string filter_type;
    std::string config;
    Transaction::Category category;
    int i{};
    while (getline(line_stream, word, ';')) {
      switch (i) {
      case 0:
        category = ParseTypeFrom(word);
        break;
      case 1:
        filter_type = word;
        break;
      case 2:
        config = word;
        break;
      default:
        break;
      }

      ++i;
    }
    if (filter_type == "Reason") {
      category_filters.push_back(
          std::make_unique<ReasonFilter>(std::regex{config}, category));
    }
    if (filter_type == "IBAN") {
      category_filters.push_back(
          std::make_unique<IbanFilter>(config, category));
    }
    if (filter_type == "PartnerName") {
      category_filters.push_back(std::make_unique<PartnerNameFilter>(
          std::regex{config, std::regex_constants::icase}, category));
    }
    if (filter_type == "Type") {
      category_filters.push_back(std::make_unique<TransactionTypeFilter>(
          std::regex{config, std::regex_constants::icase}, category));
    }
  }
  return std::move(category_filters);
}
} // namespace finance
