#pragma once

#include "trie.hxx"

#include <iostream>
#include <map>
#include <ranges>
#include <vector>

namespace prefixspan {
  namespace core {
    template<typename range_t>
    concept range = std::ranges::random_access_range<range_t> &&
      std::ranges::sized_range<range_t>;

    template<typename range_t>
    using value_t = std::ranges::range_value_t<range_t>;

    template<typename range_t, typename symbol_t>
    concept database = range<range_t> && range<value_t<range_t>> &&
      std::same_as<value_t<value_t<range_t>>, symbol_t>;

    template<typename symbol_t, database<symbol_t> database_t>
    auto project(database_t const & db, symbol_t const & symbol) {
      auto transform = std::views::transform([&symbol](auto const & sequence) {
        auto begin = std::ranges::find(sequence, symbol);
        if (begin != sequence.end()) {
          begin = std::ranges::next(begin);
        }
        return std::ranges::subrange(begin, sequence.end());
      });
      auto db_new = db | transform;
      return std::vector(db_new.begin(), db_new.end());
    };
  }; // namespace core

  template<typename symbol_t, core::database<symbol_t> database_t>
  trie<symbol_t> make(database_t const & db, std::size_t const & minsup) {
    trie<symbol_t> t;

    auto db_view = db;

    std::map<symbol_t, std::size_t> count;
    for (auto sequence : db_view) {
      for (symbol_t symbol : sequence) {
        count[symbol] += 1;
      }
    }

    for (auto [symbol, c] : count) {
      if (c >= minsup) {
        auto db_new = core::project(db_view, symbol);
        t.insert(symbol, make<symbol_t>(db_new, minsup));
      }
    }

    return t;
  }

}; // namespace prefixspan
