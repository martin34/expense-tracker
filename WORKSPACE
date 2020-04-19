workspace(name = "codewars")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Googletest
http_archive(
    name = "googletest",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
    urls = ["https://github.com/google/googletest/archive/release-1.10.0.zip"],
)

# Benchmark
http_archive(
    name = "benchmark",
    sha256 = "2d22dd3758afee43842bb504af1a8385cccb3ee1f164824e4837c1c1b04d92a0",
    strip_prefix = "benchmark-1.5.0",
    urls = ["https://github.com/google/benchmark/archive/v1.5.0.zip"],
)

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# Boost
git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# Qt
git_repository(
    name = "com_justbuchanan_rules_qt",
    branch = "master",
    remote = "https://github.com/martin34/bazel_rules_qt.git",
)

new_local_repository(
    name = "qt",
    build_file = "@com_justbuchanan_rules_qt//:qt.BUILD",
    path = "/opt/Qt5.9.1/5.9.1/gcc_64/include",
)

new_local_repository(
    name = "qt_libs",
    build_file = "@com_justbuchanan_rules_qt//:qt_libs.BUILD",
    path = "/usr/lib/x86_64-linux-gnu",
)
