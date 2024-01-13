# Build file for Backend Project

# Binary for Server.cpp
cc_binary(
    name = "server",
    srcs = ["src/Server.cpp"],
    copts = [
        "-I/usr/local/include/crow",
        "-I/usr/include/x86_64-linux-gnu/nlohmann",
        "-I/usr/local/include/openssl",
    ],
    linkopts = [
        "-lpthread",
        "-lssl",
        "-lcrypto",
        "-L/usr/local/lib",
    ],  # Link with pthread library
    toolchains = ["@local_config_cc//:toolchain"],
    deps = ["//server/utils:json-parser",
    "//server/utils:autho"]
)
