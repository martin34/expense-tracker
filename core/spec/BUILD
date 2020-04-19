cc_library(
    name = "test_utils",
    testonly = True,
    srcs = ["test_utils.cpp"],
    hdrs = ["test_utils.h"],
    visibility = [
        "//cmd/utils/spec:__subpackages__",
        "//core/stream:__subpackages__",
        "//tracker/spec:__subpackages__",
    ],
    deps = ["//core"],
)

cc_test(
    name = "spec",
    srcs = [
        "course_spec.cpp",
        "date_spec.cpp",
        "pivot_table_spec.cpp",
        "transaction_spec.cpp",
        "transactions_spec.cpp",
    ],
    deps = [
        ":test_utils",
        "//core",
        "@googletest//:gtest_main",
        "@googletest//:gtest_sample_lib",
    ],
)
