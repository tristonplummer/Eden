#pragma once

#include <functional>
#include <future>
#include <memory>

// This method of leveraging std::async to fire and forget was taken
// from https://stackoverflow.com/a/56834117
// The issue here is that the destructor of std::future waits for the result
// to arrive, thus ends up performing it in a synchronous manner, but all we want
// is to be able to fire some packets on a separate thread, and we don't care
// about the result of those packet handlers (as it's void).
#define ASYNC(fun, ...)                                                              \
    {                                                                                \
        auto ptr = std::make_shared<std::future<void>>();                            \
        *ptr     = std::async(std::launch::async, [ptr, fun] { fun(__VA_ARGS__); }); \
    }
