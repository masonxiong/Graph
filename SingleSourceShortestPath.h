#ifndef SINGLESOURCESHORTESTPATH_H
#define SINGLESOURCESHORTESTPATH_H

#include "GraphConcepts.h"

namespace graph {
    template <typename Method = Controller, typename Weight = Unweighted, typename Merge = std::plus<Weight>, typename Compare = std::less<Weight>, Weight Identity = Weight(), Weight Infinity = std::numeric_limits<Weight>::max()>
    class SingleSourceShortestPath {
        static_assert(!std::is_same_v<Method, Method>, "You must specify an algorithm type. Use SingleSourceShortestPath<>::NaiveBellmanFord, OptimizedBellmanFord, NaiveDijkstra, or OptimizedDijkstra.");
    };

    template <typename Weight, typename Merge, typename Compare, Weight Identity, Weight Infinity>
    class SingleSourceShortestPath<Controller, Weight, Merge, Compare, Identity, Infinity> {
    public:
        struct NaiveBellmanFord {};
        struct OptimizedBellmanFord {};
        struct NaiveDijkstra {};
        struct OptimizedDijkstra {};
    };

    template <typename Weight, typename Merge, typename Compare, Weight Identity, Weight Infinity>
    class SingleSourceShortestPath<SingleSourceShortestPath<>::NaiveBellmanFord, Weight, Merge, Compare, Identity, Infinity> {
        bool m_valid;
        std::vector<Weight> m_distance;
    
    public:
        constexpr ~SingleSourceShortestPath() = default;
        constexpr SingleSourceShortestPath(const SingleSourceShortestPath& other) = default;
        constexpr SingleSourceShortestPath(SingleSourceShortestPath&& other) = default;
        constexpr auto operator=(const SingleSourceShortestPath& other) -> SingleSourceShortestPath& = default;
        constexpr auto operator=(SingleSourceShortestPath&& other) -> SingleSourceShortestPath& = default;

        constexpr SingleSourceShortestPath() {
            reset();
        }

        constexpr SingleSourceShortestPath(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) {
            reset(graph, source, merge, compare);
        }

        constexpr auto reset() -> void {
            m_distance.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            m_distance.assign(n, Infinity), m_distance[source] = Identity, m_valid = true;
            std::vector<std::tuple<sizeType, sizeType, Weight>> edges;
            for (sizeType u = 0; u != n; ++u)
                for (auto e : graph[u])
                    edges.emplace_back(u, sizeType(std::get<0>(e)), std::get<1>(e));
            for (sizeType round = n; round-- && m_valid; )
                for (m_valid = false; auto [u, v, w] : edges)
                    if (m_distance[u] != Infinity && compare(merge(m_distance[u], w), m_distance[v]))
                        m_distance[v] = merge(m_distance[u], w), m_valid = true;
            m_valid = !m_valid;
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()() const -> const std::vector<Weight>& {
            return m_distance;
        }

        constexpr auto operator()() -> std::vector<Weight>& {
            return m_distance;
        }
    };

    template <typename Weight, typename Merge, typename Compare, Weight Identity, Weight Infinity>
    class SingleSourceShortestPath<SingleSourceShortestPath<>::OptimizedBellmanFord, Weight, Merge, Compare, Identity, Infinity> {
        bool m_valid;
        std::vector<Weight> m_distance;
    
    public:
        constexpr ~SingleSourceShortestPath() = default;
        constexpr SingleSourceShortestPath(const SingleSourceShortestPath& other) = default;
        constexpr SingleSourceShortestPath(SingleSourceShortestPath&& other) = default;
        constexpr auto operator=(const SingleSourceShortestPath& other) -> SingleSourceShortestPath& = default;
        constexpr auto operator=(SingleSourceShortestPath&& other) -> SingleSourceShortestPath& = default;

        constexpr SingleSourceShortestPath() {
            reset();
        }

        constexpr SingleSourceShortestPath(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) {
            reset(graph, source, merge, compare);
        }

        constexpr auto reset() -> void {
            m_distance.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            m_distance.assign(n, Infinity), m_distance[source] = Identity, m_valid = true;
            std::vector<bool> visited(n);
            std::vector<sizeType> count(n), queue(n);
            auto front = queue.begin(), back = queue.begin();
            for (*back++ = source, ++count[source]; front != back; ) {
                sizeType u = *front;
                ++front == queue.end() && (front = queue.begin(), 1), visited[u] = false;
                for (auto e : graph[u]) {
                    sizeType v = sizeType(std::get<0>(e));
                    if (compare(merge(m_distance[u], std::get<1>(e)), m_distance[v])) {
                        m_distance[v] = merge(m_distance[u], std::get<1>(e));
                        if (!visited[v]) {
                            if (*back = v, visited[v] = true, ++count[v] >= n)
                                return void(m_valid = false);
                            ++back == queue.end() && (back = queue.begin(), 1);
                        }
                    }
                }
            }
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()() const -> const std::vector<Weight>& {
            return m_distance;
        }

        constexpr auto operator()() -> std::vector<Weight>& {
            return m_distance;
        }
    };

    template <typename Weight, typename Merge, typename Compare, Weight Identity, Weight Infinity>
    class SingleSourceShortestPath<SingleSourceShortestPath<>::NaiveDijkstra, Weight, Merge, Compare, Identity, Infinity> {
        bool m_valid;
        std::vector<Weight> m_distance;
    
    public:
        constexpr ~SingleSourceShortestPath() = default;
        constexpr SingleSourceShortestPath(const SingleSourceShortestPath& other) = default;
        constexpr SingleSourceShortestPath(SingleSourceShortestPath&& other) = default;
        constexpr auto operator=(const SingleSourceShortestPath& other) -> SingleSourceShortestPath& = default;
        constexpr auto operator=(SingleSourceShortestPath&& other) -> SingleSourceShortestPath& = default;

        constexpr SingleSourceShortestPath() {
            reset();
        }

        constexpr SingleSourceShortestPath(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) {
            reset(graph, source, merge, compare);
        }

        constexpr auto reset() -> void {
            m_distance.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            std::vector<bool> visited(n);
            m_distance.assign(n, Infinity), m_distance[source] = Identity, m_valid = true;
            for (sizeType u = 0; u != n && m_valid; ++u)
                for (auto e : graph[u])
                    m_valid = m_valid && !compare(std::get<1>(e), Identity);
            if (m_valid && n) {
                for (sizeType round = n; round--; ) {
                    sizeType u = sizeType(-1);
                    for (sizeType v = 0; v != n; ++v)
                        if (!visited[v] && (!~u || compare(m_distance[v], m_distance[u])))
                            u = v;
                    if (visited[u] = true, m_distance[u] != Infinity)
                        for (auto e : graph[u])
                            if (compare(merge(m_distance[u], std::get<1>(e)), m_distance[sizeType(std::get<0>(e))]))
                                m_distance[sizeType(std::get<0>(e))] = merge(m_distance[u], std::get<1>(e));
                }
            }
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()() const -> const std::vector<Weight>& {
            return m_distance;
        }

        constexpr auto operator()() -> std::vector<Weight>& {
            return m_distance;
        }
    };

    template <typename Weight, typename Merge, typename Compare, Weight Identity, Weight Infinity>
    class SingleSourceShortestPath<SingleSourceShortestPath<>::OptimizedDijkstra, Weight, Merge, Compare, Identity, Infinity> {
        bool m_valid;
        std::vector<Weight> m_distance;
    
    public:
        constexpr ~SingleSourceShortestPath() = default;
        constexpr SingleSourceShortestPath(const SingleSourceShortestPath& other) = default;
        constexpr SingleSourceShortestPath(SingleSourceShortestPath&& other) = default;
        constexpr auto operator=(const SingleSourceShortestPath& other) -> SingleSourceShortestPath& = default;
        constexpr auto operator=(SingleSourceShortestPath&& other) -> SingleSourceShortestPath& = default;

        constexpr SingleSourceShortestPath() {
            reset();
        }

        constexpr SingleSourceShortestPath(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) {
            reset(graph, source, merge, compare);
        }

        constexpr auto reset() -> void {
            m_distance.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph, sizeType source, Merge merge = Merge(), Compare compare = Compare()) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            m_valid = true;
            for (sizeType u = 0; u != n && m_valid; ++u)
                for (auto e : graph[u])
                    m_valid = m_valid && !compare(std::get<1>(e), Identity);
            if (m_valid && n) {
                sizeType delta = n == 1 ? 2 : sizeType(1) << std::bit_width(n - 1);
                std::vector<std::pair<Weight, sizeType>> tree(delta << 1, std::pair(Infinity, 0));
                auto pushup = [compare](auto left, auto right) { return compare(left.first, right.first) ? left : right; };
                auto modify = [&tree, delta, pushup](auto position, auto value) { for (position += delta, tree[position] = std::pair(value, position - delta); position >>= 1; tree[position] = pushup(tree[position << 1], tree[position << 1 | 1])); };
                m_distance.assign(n, Infinity), modify(source, m_distance[source] = Identity);
                for (sizeType round = n; round--; ) {
                    sizeType u = tree[1].second;
                    if (tree[1].first == Infinity)
                        break;
                    for (modify(u, Infinity); auto e : graph[u])
                        if (compare(merge(m_distance[u], std::get<1>(e)), m_distance[sizeType(std::get<0>(e))]))
                            modify(sizeType(std::get<0>(e)), m_distance[sizeType(std::get<0>(e))] = merge(m_distance[u], std::get<1>(e)));
                }
            }
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()() const -> const std::vector<Weight>& {
            return m_distance;
        }

        constexpr auto operator()() -> std::vector<Weight>& {
            return m_distance;
        }
    };
}

#endif