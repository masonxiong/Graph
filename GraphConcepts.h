#ifndef GRAPHCONCEPTS_H
#define GRAPHCONCEPTS_H

#include "Foundation.h"

namespace graph {
    template <typename edge>
    concept UnweightedEdge = requires(edge edgeObject) {
        typename std::tuple_size<std::remove_cvref_t<edge>>::type;
        requires std::tuple_size_v<std::remove_cvref_t<edge>> == 2;
        { std::get<0>(edgeObject) } -> std::convertible_to<vertexType>;
        { std::get<1>(edgeObject) } -> std::convertible_to<vertexType>;
    };

    template <typename edge>
    concept WeightedEdge = requires(edge edgeObject) {
        typename std::tuple_size<std::remove_cvref_t<edge>>::type;
        requires std::tuple_size_v<std::remove_cvref_t<edge>> == 3;
        { std::get<0>(edgeObject) } -> std::convertible_to<vertexType>;
        { std::get<1>(edgeObject) } -> std::convertible_to<vertexType>;
    };

    template <typename edge>
    concept Edge = UnweightedEdge<edge> || WeightedEdge<edge>;

    template <typename graph>
    concept UnweightedGraph = requires(graph graphObject, vertexType vertex) {
        { graphObject[vertex] } -> std::ranges::range;
        requires std::convertible_to<std::ranges::range_value_t<decltype(graphObject[vertex])>, vertexType>;
    };

    template <typename graph>
    concept WeightedGraph = requires(graph graphObject, vertexType vertex) {
        { graphObject[vertex] } -> std::ranges::range;
        requires requires(std::ranges::range_value_t<decltype(graphObject[vertex])> edge) { typename std::tuple_size<std::remove_cvref_t<decltype(edge)>>::type; requires std::tuple_size_v<std::remove_cvref_t<decltype(edge)>> == 2; { std::get<0>(edge) } -> std::convertible_to<vertexType>; };
    };

    template <typename graph>
    concept Graph = UnweightedGraph<graph> || WeightedGraph<graph>;
}

#endif