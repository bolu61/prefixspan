#pragma once

#include "./trie.hxx"

#include <map>
#include <ranges>

namespace prefixspan {
  // todo: implement function of projected database

  template<typename database_t, typename symbol_t>
  requires std::ranges::range<database_t> &&
    std::ranges::range<std::ranges::range_value_t<database_t>> &&
    std::same_as<
             std::ranges::range_value_t<std::ranges::range_value_t<database_t>>,
             symbol_t>
  trie<symbol_t> make(database_t const & database, std::size_t const & minsup) {
    // todo @bolu61
    trie<symbol_t> t;

    std::map<symbol_t, std::size_t> count;
    for (auto sequence : database) {
      for (symbol_t symbol : sequence) {
        count[symbol] += 1;
      }
    }

    for (auto [symbol, c] : count) {
      if (c >= minsup) {
        // todo: project, recurse, then insert
        t.insert(symbol);
      }
    }

    return t;
  }

}; // namespace prefixspan
