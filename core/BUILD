cc_library(
    name = "core",
    srcs = [
        "course.cpp",
        "date.cpp",
        "pivot_table.cpp",
        "transaction.cpp",
        "transactions.cpp",
        "utils.cpp",
    ],
    hdrs = [
        "course.h",
        "date.h",
        "pivot_table.h",
        "transaction.h",
        "transactions.h",
        "utils.h",
    ],
    visibility = [
        ":__subpackages__",
        "//cmd:__subpackages__",
        "//parser:__subpackages__",
        "//tracker:__subpackages__",
    ],
    deps = [
        "@boost//:date_time",
    ],
)
