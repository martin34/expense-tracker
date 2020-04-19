cc_library(
    name = "tracker",
    srcs = [
        "tracker.cpp",
    ],
    hdrs = [
        "tracker.h",
    ],
    data = [
        ":filter_config",
    ],
    visibility = [
        ":__subpackages__",
        "//cmd:__pkg__",
        "//gui:__subpackages__",
    ],
    deps = [
        "//core",
        "//core/categories",
        "//core/stream",
        "//parser",
    ],
)

filegroup(
    name = "filter_config",
    data = [".category_config"],
)
