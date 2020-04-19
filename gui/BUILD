load("@com_justbuchanan_rules_qt//:qt.bzl", "qt_cc_library", "qt_ui_library")

qt_cc_library(
    name = "my_widget",
    srcs = [
        "bar_chart.cpp",
        "category_change_widget.cpp",
        "category_string_completer.cpp",
        "chart.cpp",
        "chart.h",
        "chart_view.cpp",
        "chart_view.h",
        "color_map.h",
        "course_chart.cpp",
        "mainwindow.cpp",
        "pie_chart.cpp",
        "report_widget.cpp",
        "transaction_table_model.cpp",
        "transaction_table_model.h",
        "transaction_table_view.cpp",
        "transaction_table_view.h",
        "utils.cpp",
        "utils.h",
    ],
    hdrs = [
        "bar_chart.h",
        "category_change_widget.h",
        "category_string_completer.h",
        "course_chart.h",
        "mainwindow.h",
        "pie_chart.h",
        "report_widget.h",
    ],
    visibility = [
        ":__subpackages__",
    ],
    deps = [
        ":mainwindow",
        "//tracker",
        "@qt//:qt_charts",
        "@qt//:qt_widgets",
        "@qt_libs//:qt_lib_charts",
        "@qt_libs//:qt_lib_widget",
    ],
)

qt_ui_library(
    name = "mainwindow",
    ui = "mainwindow.ui",
    deps = [
        "@qt//:qt_charts",
        "@qt//:qt_widgets",
        "@qt_libs//:qt_lib_charts",
        "@qt_libs//:qt_lib_widget",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    copts = ["-fpic"],
    deps = [
        ":mainwindow",
        ":my_widget",
        "//cmd/arguments",
        "//tracker",
        "@qt//:qt_widgets",
    ],
)
