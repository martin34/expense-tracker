#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/transaction.h"

namespace {
using namespace ::testing;
using namespace finance;

TEST(TransactionEqualSpec, WhenDefaultEqual) {
  Transaction lhs{};
  Transaction rhs{};
  EXPECT_EQ(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentDate) {
  Transaction lhs{};
  Transaction rhs{};
  rhs.date = Date{1999, 2, 11};
  EXPECT_NE(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentAmount) {
  Transaction lhs{};
  lhs.amount = 179.85;
  Transaction rhs{};
  rhs.amount = 3179.85;
  EXPECT_NE(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentAmountSwitchedComperisonOrder) {
  Transaction rhs{};
  rhs.amount = 179.85;
  Transaction lhs{};
  lhs.amount = 3179.85;
  EXPECT_NE(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentPartner) {
  Transaction lhs{};
  Transaction rhs{};
  rhs.partner = Iban{"DE12345667"};
  EXPECT_NE(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentPartnerName) {
  Transaction lhs{};
  Transaction rhs{};
  rhs.partner_name = std::string{"Franz"};
  EXPECT_NE(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentReason) {
  Transaction lhs{};
  Transaction rhs{};
  lhs.reason = std::string{"foo"};
  EXPECT_NE(lhs, rhs);
}
TEST(TransactionEqualSpec, WhenDifferentType) {
  Transaction lhs{};
  Transaction rhs{};
  lhs.category = Transaction::Insurance;
  EXPECT_NE(lhs, rhs);
}
} // namespace