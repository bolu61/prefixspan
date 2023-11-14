#pragma once

#include <map>
#include <set>
#include <variant>
#include <ranges>

namespace prefixspan {
    template<typename node_t=std::monostate, typename edge_t=std::monostate>
    struct graph {
        using index_t = unsigned int;
        static auto insert(graph<node_t, edge_t> &g, index_t const &v, node_t value) {
            return g.node.insert({v, value});
        }
        static auto insert(graph<node_t, edge_t> & g, index_t const& u, index_t const& v, edge_t value) {
            return g.edge.insert({{u, v}, value});
        }
        static auto successors(graph<node_t, edge_t> const& g, index_t const& u) {
            return g.succ.equal_range(u);
        }

    private:
        std::map<index_t, node_t> node;
        std::map<std::pair<index_t, index_t>, edge_t> edge;
        std::multimap<index_t, index_t> succ;
    };

    template<typename node_t=std::monostate, typename edge_t=std::monostate>
    struct tree {
        static auto rooted_at(graph<node_t, edge_t>& g, graph<>::index_t const& r);
    private:
        graph<node_t, edge_t>::index_t root;
        std::shared_ptr<graph<node_t, edge_t>> graph;

    };

    template <std::ranges::random_access_range S>
    graph<unsigned int, std::ranges::range_value_t<S>> build(S const& sequence) {
        using T = std::ranges::range_value_t<S>;

        auto alphabet = std::set<T>(sequence.begin(), sequence.end());

        auto g = graph<unsigned int, T>();
        graph<unsigned int, T>::insert(g, 0, 0);

        auto worklist = std::vector<graph<>::index_t>();

        for (const T &s : sequence) {
            // asdfasdf
        }
        return g;
    };
};