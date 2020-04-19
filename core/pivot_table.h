#pragma once

#include "course.h"
#include "date.h"
#include "transaction.h"

namespace finance {

struct PivotTable {
  std::vector<Date> bins;
  std::map<Transaction::Category, std::vector<double>> values;
};

using CoursePerCategory = std::map<Transaction::Category, Course>;
PivotTable GenerateMonthlyAverageSummary(
    const CoursePerCategory &transactions_per_category, const Date &start,
    const Date &end);

std::vector<Date> CreateBinsMonthly(const Date &start, const Date &end);
std::map<Transaction::Category, std::vector<double>>
PartitionToBins(const CoursePerCategory &transactions_per_category,
                const std::vector<Date> &bins);
} // namespace finance