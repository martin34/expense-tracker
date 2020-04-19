#include <cmath>

#include "utils.h"

namespace utils {
bool DoubleEqual(double lhs, double rhs) {
  return (std::fabs((lhs - rhs)) < kPrecision);
}
} // namespace utils