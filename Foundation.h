#ifndef FOUNDATION_H
#define FOUNDATION_H

#if __cplusplus < 202002L
#error "Graph Library requires C++20 or higher."
#endif

#ifdef ENABLE_VALIDITY_CHECK
#define VALIDITY_CHECK(condition, errorType, message) if (!(condition)) throw errorType(message);
#else
#define VALIDITY_CHECK(condition, errorType, message)
#endif

#include <span>
#include <tuple>
#include <ranges>
#include <vector>
#include <string>
#include <utility>
#include <concepts>
#include <stdexcept>
#include <algorithm>
#include <numeric>

namespace graph {
    using vertexType = unsigned;
    using sizeType = unsigned;
}

#endif