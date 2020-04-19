cc_library(
    name = "parser",
    srcs = [
        "n26_parser.cpp",
        "rba_parser.cpp",
    ],
    hdrs = [
        "n26_parser.h",
        "rba_parser.h",
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
