#ifndef ALLPAIRSSHORTESTPATH_H
#define ALLPAIRSSHORTESTPATH_H

#include "PredefinedGraphs.h"
#include "SingleSourceShortestPath.h"

namespace graph {
    template <typename Method = Controller, typename Weight = Unweighted, typename Merge = std::plus<Weight>, typename Split = std::minus<Weight>, typename Compare = std::less<Weight>, Weight Identity = Weight(), Weight Infinity = std::numeric_limits<Weight>::max()>
    class AllPairsShortestPath {
        static_assert(!std::is_same_v<Method, Method>, "You must specify an algorithm type. Use AllPairsShortestPath<>::Floyd or Johnson.");
    };

    template <typename Weight, typename Merge, typename Split, typename Compare, Weight Identity, Weight Infinity>
    class AllPairsShortestPath<Controller, Weight, Merge, Split, Compare, Identity, Infinity> {
    public:
        struct Floyd {};
        struct Johnson {};
    };

    template <typename Weight, typename Merge, typename Split, typename Compare, Weight Identity, Weight Infinity>
    class AllPairsShortestPath<AllPairsShortestPath<>::Floyd, Weight, Merge, Split, Compare, Identity, Infinity> {
        bool m_valid;
        std::vector<std::vector<Weight>> m_distance;

    public:
        constexpr ~AllPairsShortestPath() = default;
        constexpr AllPairsShortestPath(const AllPairsShortestPath& other) = default;
        constexpr AllPairsShortestPath(AllPairsShortestPath&& other) = default;
        constexpr auto operator=(const AllPairsShortestPath& other) -> AllPairsShortestPath& = default;
        constexpr auto operator=(AllPairsShortestPath&& other) -> AllPairsShortestPath& = default;

        constexpr AllPairsShortestPath() {
            reset();
        }

        constexpr AllPairsShortestPath(WeightedGraph<Weight> auto const& graph, Merge merge = Merge(), Split split = Split(), Compare compare = Compare()) {
            reset(graph, merge, split, compare);
        }

        constexpr auto reset() -> void {
            m_distance.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph, Merge merge = Merge(), Split split = Split(), Compare compare = Compare()) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            m_distance.assign(n, std::vector<Weight>(n, split(Infinity, Identity))), m_valid = true;
            for (sizeType u = 0; u != n; ++u) {
                m_distance[u][u] = Identity;
                for (auto e : graph[u])
                    if (compare(std::get<1>(e), m_distance[u][sizeType(std::get<0>(e))]))
                        m_distance[u][sizeType(std::get<0>(e))] = std::get<1>(e);
            }
            for (sizeType k = 0; k != n; ++k)
                for (sizeType i = 0; i != n; ++i)
                    for (sizeType j = 0; j != n; ++j)
                        if (m_distance[i][k] != Infinity && m_distance[k][j] != Infinity && compare(merge(m_distance[i][k], m_distance[k][j]), m_distance[i][j]))
                            m_distance[i][j] = merge(m_distance[i][k], m_distance[k][j]);
            for (sizeType i = 0; i != n; ++i)
                if (compare(m_distance[i][i], Identity))
                    return void(m_valid = false);
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()(sizeType source) const -> const std::vector<Weight>& {
            return m_distance[source];
        }

        constexpr auto operator()(sizeType source) -> std::vector<Weight>& {
            return m_distance[source];
        }
    };

    template <typename Weight, typename Merge, typename Split, typename Compare, Weight Identity, Weight Infinity>
    class AllPairsShortestPath<AllPairsShortestPath<>::Johnson, Weight, Merge, Split, Compare, Identity, Infinity> {
        bool m_valid;
        std::vector<std::vector<Weight>> m_distance;

    public:
        constexpr ~AllPairsShortestPath() = default;
        constexpr AllPairsShortestPath(const AllPairsShortestPath& other) = default;
        constexpr AllPairsShortestPath(AllPairsShortestPath&& other) = default;
        constexpr auto operator=(const AllPairsShortestPath& other) -> AllPairsShortestPath& = default;
        constexpr auto operator=(AllPairsShortestPath&& other) -> AllPairsShortestPath& = default;

        constexpr AllPairsShortestPath() {
            reset();
        }

        constexpr AllPairsShortestPath(WeightedGraph<Weight> auto const& graph, Merge merge = Merge(), Split split = Split(), Compare compare = Compare()) {
            reset(graph, merge, split, compare);
        }

        constexpr auto reset() -> void {
            m_distance.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph, Merge merge = Merge(), Split split = Split(), Compare compare = Compare()) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            StaticGraph<Weight> newGraph(n + 1);
            for (sizeType u = 0; u != n; ++u) {
                for (auto e : graph[u])
                    newGraph.insertEdge(u, sizeType(std::get<0>(e)), std::get<1>(e));
                newGraph.insertEdge(n, u, Identity);
            }
            newGraph.lock();
            if (SingleSourceShortestPath<SingleSourceShortestPath<>::OptimizedBellmanFord, Weight, Merge, Compare, Identity, Infinity> height(newGraph, n, merge, compare); (m_valid = bool(height))) {
                for (sizeType u = 0; u != n; ++u)
                    for (auto& e : newGraph[u])
                        std::get<1>(e) = (height()[u] == Infinity && height()[sizeType(std::get<0>(e))] != Infinity ? Infinity : merge(std::get<1>(e), split(height()[u], height()[sizeType(std::get<0>(e))])));
                m_distance.assign(n, std::vector<Weight>());
                for (sizeType u = 0; u != n; ++u) {
                    SingleSourceShortestPath<SingleSourceShortestPath<>::OptimizedDijkstra, Weight, Merge, Compare, Identity, Infinity> distance(newGraph, u, merge, compare);
                    (m_distance[u] = std::move(distance())).pop_back();
                    for (sizeType v = 0; v != n; ++v)
                        if (m_distance[u][v] != Infinity)
                            m_distance[u][v] = merge(m_distance[u][v], split(height()[v], height()[u]));
                }
            }
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()(sizeType source) const -> const std::vector<Weight>& {
            return m_distance[source];
        }

        constexpr auto operator()(sizeType source) -> std::vector<Weight>& {
            return m_distance[source];
        }
    };
}

#endif