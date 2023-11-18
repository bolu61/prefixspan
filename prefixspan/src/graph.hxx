#pragma once

#include <map>
#include <set>
#include <variant>

namespace prefixspan {
  struct Graph {
    using index_t = unsigned int;

    static std::pair<std::set<index_t>::iterator, bool> insert(Graph & g, index_t const & u, index_t const & v){
      return g.edge[u].insert(v);
    };

    static std::set<index_t> const & successors(Graph & g, index_t const & u) {
      return g.edge[u];
    };

    private:
    std::map<index_t, std::set<index_t>> edge;
  };
} // namespace prefixspan
