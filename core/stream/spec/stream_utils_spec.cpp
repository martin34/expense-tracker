#include <sstream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/spec/test_utils.h"
#include "core/stream/utils.h"
#include "core/transaction.h"

namespace {
using namespace finance;
using namespace ::testing;

TEST(Csv, ReadWrite) {
  std::vector<Transaction> transactions{AnTransaction()
                                            .With("Some name")
                                            .WithIban("DE1234")
                                            .WithReason("Foo Bar")
                                            .WithAmount(1.)
                                            .At(Date{2018, 1, 10})
                                            .Build(),
                                        AnTransaction()
                                            .With("Other name")
                                            .WithIban("DE6789")
                                            .WithReason("other")
                                            .WithAmount(2.)
                                            .At(Date{2018, 1, 11})
                                            .Build()};
  std::stringstream stream{};

  WriteTransactionsTo(transactions, stream);
  auto actual = ReadTransactionsFromCsv(stream);

  EXPECT_THAT(actual, ContainerEq(transactions));
}

} // namespace