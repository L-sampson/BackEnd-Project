# BUILD file for utils

cc_library(
    name = "autho",
    srcs = ["AuthenticateUsers.cpp"],
    hdrs = ["AuthenticateUsers.hpp"],
    copts = ["-I/usr/include/openssl",
    "-I/usr/include/jwt-cpp",
    "-I/usr/include/picojson",
    "-std=c++20"],
    deps = ["//server/utils:keys"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "json-parser",
    srcs = ["JsonParser.cpp"],
    hdrs = ["JsonParser.hpp"],
    copts = ["-I/usr/include/x86_64-linux-gnu/nlohmann",
    "-std=c++17"],
    deps = ["//server/utils:autho"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "encrypt",
    srcs = ["Encrypt.cpp"],
    hdrs = ["Encrypt.hpp"],
    copts = ["-I/usr/include/openssl",
    "-I/usr/include/x86_64-linux-gnu/nlohmann",
    "-std=c++17"],
    deps = ["//server/utils:keys"],
    visibility = ["//visibility:public"],
)

cc_library (
    name = "keys",
    srcs = ["EncryptionKeys.cpp"],
    hdrs = ["EncryptionKeys.hpp"],
    visibility = ["//visibility:public"]
)
