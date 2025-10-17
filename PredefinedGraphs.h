#ifndef PREDEFINEDGRAPHS_H
#define PREDEFINEDGRAPHS_H

#include "GraphConcepts.h"

namespace graph {
    template <typename Weight = Unweighted>
    class DynamicGraph {
        std::vector<std::vector<std::pair<sizeType, Weight>>> m_adjacentLists;

    public:
        constexpr ~DynamicGraph() = default;
        constexpr DynamicGraph(const DynamicGraph& other) = default;
        constexpr DynamicGraph(DynamicGraph&& other) = default;
        constexpr auto operator=(const DynamicGraph& other) -> DynamicGraph& = default;
        constexpr auto operator=(DynamicGraph&& other) -> DynamicGraph& = default;

        constexpr DynamicGraph() {
            reset();
        }

        constexpr DynamicGraph(sizeType size) {
            reset(size);
        }

        constexpr auto reset() -> void {
            m_adjacentLists.clear();
        }

        constexpr auto reset(sizeType size) -> void {
            m_adjacentLists.assign(size, typename decltype(m_adjacentLists)::value_type()), std::ranges::for_each(m_adjacentLists, [](auto& element) -> void { element.reserve(16); });
        }

        constexpr auto lock() const -> void {
            return void();
        }

        constexpr auto locked() const -> bool {
            return false;
        }

        constexpr auto empty() const -> bool {
            return m_adjacentLists.empty();
        }

        constexpr auto size() const -> sizeType {
            return sizeType(m_adjacentLists.size());
        }

        constexpr auto insertEdge(sizeType source, sizeType destination, Weight weight) -> void {
            VALIDITY_CHECK(source < size(), std::out_of_range, "Source vertex out of range")
            VALIDITY_CHECK(destination < size(), std::out_of_range, "Destination vertex out of range")
            m_adjacentLists[source].emplace_back(destination, weight);
        }

        constexpr auto insertEdge(WeightedEdge<Weight> auto edge) -> void {
            auto [source, destination, weight] = edge;
            insertEdge(sizeType(source), sizeType(destination), weight);
        }

        template <typename inputIterator> requires WeightedEdge<std::iter_value_t<inputIterator>, Weight>
        constexpr auto insertEdge(inputIterator edgeFirst, inputIterator edgeLast) -> void {
            std::for_each(edgeFirst, edgeLast, [this](auto edge) -> void { insertEdge(edge); });
        }

        template <WeightedEdge<Weight> weightedEdge>
        constexpr auto insertEdge(std::initializer_list<weightedEdge> edges) -> void {
            insertEdge(edges.begin(), edges.end());
        }

        template <std::ranges::range sequenceContainer> requires WeightedEdge<std::ranges::range_value_t<sequenceContainer>, Weight>
        constexpr auto insertEdge(const sequenceContainer& edges) -> void {
            insertEdge(std::ranges::begin(edges), std::ranges::end(edges));
        }

        constexpr auto operator[](sizeType vertex) -> decltype(m_adjacentLists)::value_type& {
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return m_adjacentLists[vertex];
        }

        constexpr auto operator[](sizeType vertex) const -> const decltype(m_adjacentLists)::value_type& {
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return m_adjacentLists[vertex];
        }
    };

    template <>
    class DynamicGraph<Unweighted> {
        std::vector<std::vector<sizeType>> m_adjacentLists;

    public:
        constexpr ~DynamicGraph() = default;
        constexpr DynamicGraph(const DynamicGraph& other) = default;
        constexpr DynamicGraph(DynamicGraph&& other) = default;
        constexpr auto operator=(const DynamicGraph& other) -> DynamicGraph& = default;
        constexpr auto operator=(DynamicGraph&& other) -> DynamicGraph& = default;

        constexpr DynamicGraph() {
            reset();
        }

        constexpr DynamicGraph(sizeType size) {
            reset(size);
        }

        constexpr auto reset() -> void {
            m_adjacentLists.clear();
        }

        constexpr auto reset(sizeType size) -> void {
            m_adjacentLists.assign(size, typename decltype(m_adjacentLists)::value_type()), std::ranges::for_each(m_adjacentLists, [](auto& element) -> void { element.reserve(16); });
        }

        constexpr auto lock() const -> void {
            return void();
        }

        constexpr auto locked() const -> bool {
            return false;
        }

        constexpr auto empty() const -> bool {
            return m_adjacentLists.empty();
        }

        constexpr auto size() const -> sizeType {
            return sizeType(m_adjacentLists.size());
        }

        constexpr auto insertEdge(sizeType source, sizeType destination) -> void {
            VALIDITY_CHECK(source < size(), std::out_of_range, "Source vertex out of range")
            VALIDITY_CHECK(destination < size(), std::out_of_range, "Destination vertex out of range")
            m_adjacentLists[source].emplace_back(destination);
        }

        constexpr auto insertEdge(UnweightedEdge auto edge) -> void {
            auto [source, destination] = edge;
            insertEdge(sizeType(source), sizeType(destination));
        }

        template <typename inputIterator> requires UnweightedEdge<std::iter_value_t<inputIterator>>
        constexpr auto insertEdge(inputIterator edgeFirst, inputIterator edgeLast) -> void {
            std::for_each(edgeFirst, edgeLast, [this](auto edge) -> void { insertEdge(edge); });
        }

        template <UnweightedEdge unweightedEdge>
        constexpr auto insertEdge(std::initializer_list<unweightedEdge> edges) -> void {
            insertEdge(edges.begin(), edges.end());
        }

        template <std::ranges::range sequenceContainer> requires UnweightedEdge<std::ranges::range_value_t<sequenceContainer>>
        constexpr auto insertEdge(const sequenceContainer& edges) -> void {
            insertEdge(std::ranges::begin(edges), std::ranges::end(edges));
        }

        constexpr auto operator[](sizeType vertex) -> decltype(m_adjacentLists)::value_type& {
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return m_adjacentLists[vertex];
        }

        constexpr auto operator[](sizeType vertex) const -> const decltype(m_adjacentLists)::value_type& {
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return m_adjacentLists[vertex];
        }
    };

    template <typename Weight = Unweighted>
    class StaticGraph {
        bool m_locked;
        std::vector<sizeType> m_degree;
        std::vector<std::pair<sizeType, Weight>> m_adjacentLists;
        std::vector<std::tuple<sizeType, sizeType, Weight>> m_edges;

    public:
        constexpr ~StaticGraph() = default;
        constexpr StaticGraph(const StaticGraph& other) = default;
        constexpr StaticGraph(StaticGraph&& other) = default;
        constexpr auto operator=(const StaticGraph& other) -> StaticGraph& = default;
        constexpr auto operator=(StaticGraph&& other) -> StaticGraph& = default;

        constexpr StaticGraph() {
            reset();
        }

        constexpr StaticGraph(sizeType size) {
            reset(size);
        }

        constexpr auto reset() -> void {
            m_locked = false, m_degree.clear(), m_adjacentLists.clear(), m_edges.clear(), m_edges.reserve(1u << 20);
        }

        constexpr auto reset(sizeType size) -> void {
            m_locked = false, m_degree.assign(size + 1, 0), m_adjacentLists.clear(), m_edges.clear(), m_edges.reserve(1u << 20);
        }

        constexpr auto lock() -> void {
            m_locked = true, m_adjacentLists.resize(m_edges.size()), std::partial_sum(m_degree.begin(), m_degree.end(), m_degree.begin()), std::ranges::for_each(m_edges, [this](auto edge) { m_adjacentLists[--m_degree[std::get<0>(edge)]] = std::pair(std::get<1>(edge), std::move(std::get<2>(edge))); }), m_edges.clear();
        }

        constexpr auto locked() const -> bool {
            return m_locked;
        }

        constexpr auto empty() const -> bool {
            return m_degree.empty();
        }

        constexpr auto size() const -> sizeType {
            return m_degree.empty() ? sizeType(0) : sizeType(m_degree.size() - 1);
        }

        constexpr auto insertEdge(sizeType source, sizeType destination, Weight weight) -> void {
            VALIDITY_CHECK(!m_locked, std::logic_error, "Cannot insert edge after lock")
            VALIDITY_CHECK(source < size(), std::out_of_range, "Source vertex out of range")
            VALIDITY_CHECK(destination < size(), std::out_of_range, "Destination vertex out of range")
            m_edges.emplace_back(source, destination, weight), ++m_degree[source];
        }

        constexpr auto insertEdge(WeightedEdge<Weight> auto edge) -> void {
            auto [source, destination, weight] = edge;
            insertEdge(sizeType(source), sizeType(destination), weight);
        }

        template <typename inputIterator> requires WeightedEdge<std::iter_value_t<inputIterator>, Weight>
        constexpr auto insertEdge(inputIterator edgeFirst, inputIterator edgeLast) -> void {
            std::for_each(edgeFirst, edgeLast, [this](auto edge) -> void { insertEdge(edge); });
        }

        template <WeightedEdge<Weight> weightedEdge>
        constexpr auto insertEdge(std::initializer_list<weightedEdge> edges) -> void {
            insertEdge(edges.begin(), edges.end());
        }

        template <std::ranges::range sequenceContainer> requires WeightedEdge<std::ranges::range_value_t<sequenceContainer>, Weight>
        constexpr auto insertEdge(const sequenceContainer& edges) -> void {
            insertEdge(std::ranges::begin(edges), std::ranges::end(edges));
        }

        constexpr auto operator[](sizeType vertex) -> std::span<typename decltype(m_adjacentLists)::value_type> {
            VALIDITY_CHECK(m_locked, std::logic_error, "Graph must be locked before access")
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return std::span<typename decltype(m_adjacentLists)::value_type>(m_adjacentLists.data() + m_degree[vertex], m_adjacentLists.data() + m_degree[vertex + 1]);
        }

        constexpr auto operator[](sizeType vertex) const -> std::span<const typename decltype(m_adjacentLists)::value_type> {
            VALIDITY_CHECK(m_locked, std::logic_error, "Graph must be locked before access")
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return std::span<const typename decltype(m_adjacentLists)::value_type>(m_adjacentLists.data() + m_degree[vertex], m_adjacentLists.data() + m_degree[vertex + 1]);
        }
    };

    template <>
    class StaticGraph<Unweighted> {
        bool m_locked;
        std::vector<sizeType> m_degree;
        std::vector<sizeType> m_adjacentLists;
        std::vector<std::pair<sizeType, sizeType>> m_edges;

    public:
        constexpr ~StaticGraph() = default;
        constexpr StaticGraph(const StaticGraph& other) = default;
        constexpr StaticGraph(StaticGraph&& other) = default;
        constexpr auto operator=(const StaticGraph& other) -> StaticGraph& = default;
        constexpr auto operator=(StaticGraph&& other) -> StaticGraph& = default;

        constexpr StaticGraph() {
            reset();
        }

        constexpr StaticGraph(sizeType size) {
            reset(size);
        }

        constexpr auto reset() -> void {
            m_locked = false, m_degree.clear(), m_adjacentLists.clear(), m_edges.clear(), m_edges.reserve(1u << 20);
        }

        constexpr auto reset(sizeType size) -> void {
            m_locked = false, m_degree.assign(size + 1, 0), m_adjacentLists.clear(), m_edges.clear(), m_edges.reserve(1u << 20);
        }

        constexpr auto lock() -> void {
            m_locked = true, m_adjacentLists.resize(m_edges.size()), std::partial_sum(m_degree.begin(), m_degree.end(), m_degree.begin()), std::ranges::for_each(m_edges, [this](auto edge) { m_adjacentLists[--m_degree[edge.first]] = edge.second; }), m_edges.clear();
        }

        constexpr auto locked() const -> bool {
            return m_locked;
        }

        constexpr auto empty() const -> bool {
            return m_degree.empty();
        }

        constexpr auto size() const -> sizeType {
            return m_degree.empty() ? sizeType(0) : sizeType(m_degree.size() - 1);
        }

        constexpr auto insertEdge(sizeType source, sizeType destination) -> void {
            VALIDITY_CHECK(!m_locked, std::logic_error, "Cannot insert edge after lock")
            VALIDITY_CHECK(source < size(), std::out_of_range, "Source vertex out of range")
            VALIDITY_CHECK(destination < size(), std::out_of_range, "Destination vertex out of range")
            m_edges.emplace_back(source, destination), ++m_degree[source];
        }

        constexpr auto insertEdge(UnweightedEdge auto edge) -> void {
            auto [source, destination] = edge;
            insertEdge(sizeType(source), sizeType(destination));
        }

        template <typename inputIterator> requires UnweightedEdge<std::iter_value_t<inputIterator>>
        constexpr auto insertEdge(inputIterator edgeFirst, inputIterator edgeLast) -> void {
            std::for_each(edgeFirst, edgeLast, [this](auto edge) -> void { insertEdge(edge); });
        }

        template <UnweightedEdge unweightedEdge>
        constexpr auto insertEdge(std::initializer_list<unweightedEdge> edges) -> void {
            insertEdge(edges.begin(), edges.end());
        }

        template <std::ranges::range sequenceContainer> requires UnweightedEdge<std::ranges::range_value_t<sequenceContainer>>
        constexpr auto insertEdge(const sequenceContainer& edges) -> void {
            insertEdge(std::ranges::begin(edges), std::ranges::end(edges));
        }

        constexpr auto operator[](sizeType vertex) -> std::span<typename decltype(m_adjacentLists)::value_type> {
            VALIDITY_CHECK(m_locked, std::logic_error, "Graph must be locked before access")
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return std::span<typename decltype(m_adjacentLists)::value_type>(m_adjacentLists.data() + m_degree[vertex], m_adjacentLists.data() + m_degree[vertex + 1]);
        }

        constexpr auto operator[](sizeType vertex) const -> std::span<const typename decltype(m_adjacentLists)::value_type> {
            VALIDITY_CHECK(m_locked, std::logic_error, "Graph must be locked before access")
            VALIDITY_CHECK(vertex < size(), std::out_of_range, "Vertex index out of range")
            return std::span<const typename decltype(m_adjacentLists)::value_type>(m_adjacentLists.data() + m_degree[vertex], m_adjacentLists.data() + m_degree[vertex + 1]);
        }
    };
}

#endif