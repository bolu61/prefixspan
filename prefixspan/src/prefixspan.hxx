#pragma once

#include "./graph.hxx"
#include "./ref.hxx"
#include "./tree.hxx"

#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <variant>

namespace prefixspan {

  template<typename symbol_t>
  class PrefixTree {
    Tree<symbol_t> t;

    public:

    using index_t = Tree<symbol_t>::index_t;
  };

  namespace core {
    template<typename range_t, typename value_t>
    concept range = std::ranges::random_access_range<range_t> &&
      std::same_as<std::ranges::range_value_t<range_t>, value_t>;
  }; // namespace core

  template<typename symbol_t>
  PrefixTree<symbol_t> make(auto & sequence) {
    // todo @bolu61
  }

}; // namespace prefixspan
