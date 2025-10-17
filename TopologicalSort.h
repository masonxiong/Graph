#ifndef TOPOLOGICALSORT_H
#define TOPOLOGICALSORT_H

#include "GraphConcepts.h"

namespace graph {
    template <typename Weight = Unweighted>
    class TopologicalSort {
        bool m_valid;
        std::vector<sizeType> order;
    
    public:
        constexpr ~TopologicalSort() = default;
        constexpr TopologicalSort(const TopologicalSort& other) = default;
        constexpr TopologicalSort(TopologicalSort&& other) = default;
        constexpr auto operator=(const TopologicalSort& other) -> TopologicalSort& = default;
        constexpr auto operator=(TopologicalSort&& other) -> TopologicalSort& = default;

        constexpr TopologicalSort() {
            reset();
        }

        constexpr TopologicalSort(WeightedGraph<Weight> auto const& graph) {
            reset(graph);
        }

        constexpr auto reset() -> void {
            order.clear(), m_valid = true;
        }

        constexpr auto reset(WeightedGraph<Weight> auto const& graph) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            std::vector<sizeType> degree(n), queue(n);
            auto front = queue.begin(), back = queue.begin();
            for (sizeType u = 0; u != n; ++u)
                for (auto e : graph[u])
                    ++degree[sizeType(std::get<0>(e))];
            for (sizeType u = 0; u != n; ++u)
                if (!degree[u])
                    *back++ = u;
            for (order.reserve(n); front != back; ) {
                sizeType u = *front++;
                order.push_back(u);
                for (auto e : graph[u])
                    if (!--degree[sizeType(std::get<0>(e))])
                        *back++ = sizeType(std::get<0>(e));
            }
            m_valid = order.size() == n;
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()() const -> const std::vector<sizeType>& {
            return order;
        }

        constexpr auto operator()() -> std::vector<sizeType>& {
            return order;
        }
    };

    template <>
    class TopologicalSort<Unweighted> {
        bool m_valid;
        std::vector<sizeType> order;
    
    public:
        constexpr ~TopologicalSort() = default;
        constexpr TopologicalSort(const TopologicalSort& other) = default;
        constexpr TopologicalSort(TopologicalSort&& other) = default;
        constexpr auto operator=(const TopologicalSort& other) -> TopologicalSort& = default;
        constexpr auto operator=(TopologicalSort&& other) -> TopologicalSort& = default;

        constexpr TopologicalSort() {
            reset();
        }

        constexpr TopologicalSort(UnweightedGraph auto const& graph) {
            reset(graph);
        }

        constexpr auto reset() -> void {
            order.clear(), m_valid = true;
        }

        constexpr auto reset(UnweightedGraph auto const& graph) -> void {
            sizeType n = sizeType(std::ranges::size(graph));
            std::vector<sizeType> degree(n), queue(n);
            auto front = queue.begin(), back = queue.begin();
            for (sizeType u = 0; u != n; ++u)
                for (auto v : graph[u])
                    ++degree[sizeType(v)];
            for (sizeType u = 0; u != n; ++u)
                if (!degree[u])
                    *back++ = u;
            for (order.reserve(n); front != back; ) {
                sizeType u = *front++;
                order.push_back(u);
                for (auto v : graph[u])
                    if (!--degree[sizeType(v)])
                        *back++ = sizeType(v);
            }
            m_valid = order.size() == n;
        }

        constexpr operator bool() const {
            return m_valid;
        }

        constexpr auto operator()() const -> const std::vector<sizeType>& {
            return order;
        }

        constexpr auto operator()() -> std::vector<sizeType>& {
            return order;
        }
    };
}

#endif