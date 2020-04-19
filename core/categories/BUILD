cc_library(
    name = "categories",
    srcs = [
        "category_filter_factory.cpp",
        "i_category_filter.cpp",
    ],
    hdrs = [
        "category_filter_factory.h",
        "i_category_filter.h",
        "iban_filter.h",
        "partner_name_filter.h",
        "reason_filter.h",
        "transactiontype_filter.h",
    ],
    visibility = [
        ":__subpackages__",
        "//tracker:__subpackages__",
    ],
    deps = [
        "//core",
        "//core/stream",
    ],
)
