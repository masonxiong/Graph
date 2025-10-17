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

#include <bit>
#include <span>
#include <tuple>
#include <deque>
#include <limits>
#include <ranges>
#include <vector>
#include <string>
#include <numeric>
#include <utility>
#include <concepts>
#include <stdexcept>
#include <algorithm>

namespace graph {
    using sizeType = unsigned;
    using differenceType = signed;

    struct Controller {};
    struct Unweighted {};
}

#endif