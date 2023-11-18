#pragma once

#include "ref.hxx"

#include <map>
#include <memory>

namespace prefixspan {
  template<typename value_t>
  class Tree;

  template<typename value_t>
  class Forest : public std::enable_shared_from_this<Forest<value_t>> {
    friend Tree<value_t>;
    std::map<Graph::index_t, value_t> g;
    std::set<Graph::index_t> roots;
    Graph::index_t count = 0;

    Forest() : g(), roots(){};

    public:

    static ref<Forest> make() {
      return std::make_shared<Forest<value_t>>();
    }

    Tree<value_t> tree() {
      return Tree(this->shared_from_this(), this->count++);
    }
  };

  template<typename value_t>
  class Tree {
    friend class Forest<value_t>;
    ref<Forest<value_t>> f;
    Graph::index_t i;

    public:

    using index_t = Graph::index_t;

    static std::set<index_t> children(Tree<value_t> & t) {
      return Graph::successors(t->f->g);
    }
  };
}; // namespace prefixspan
