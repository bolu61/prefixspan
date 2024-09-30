#pragma once

#include "trie.hxx"

#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace prefixspan {
  namespace core {
    namespace dbs {
      template<typename sequence_type>
      using value = std::ranges::range_value_t<sequence_type>;

      template<typename sequence_type>
      using iterator = std::ranges::iterator_t<sequence_type>;

      template<typename sequence_type, typename symbol>
      concept sequence = std::ranges::random_access_range<sequence_type> &&
        std::ranges::sized_range<sequence_type> &&
        std::same_as<value<sequence_type>, symbol>;

      template<typename database_type, typename symbol>
      concept database = sequence<database_type, value<database_type>> &&
        sequence<value<database_type>, symbol>;

    } // namespace dbs

    template<typename symbol, dbs::database<symbol> database>
    auto project(database const & db, symbol const & key) {
      using iterator = dbs::iterator<const dbs::value<database>>;
      std::vector<std::ranges::subrange<iterator>> out;
      for (dbs::sequence<symbol> auto const & seq : db) {
        iterator it = std::ranges::find(seq, key);
        if (it != seq.end()) {
          out.push_back(std::ranges::subrange(++it, seq.end()));
        }
      }
      return out;
    };

    template<typename symbol>
    std::vector<symbol> unique(dbs::sequence<symbol> auto const & seq) {
      std::vector<symbol> out(std::ranges::begin(seq), std::ranges::end(seq));
      std::ranges::sort(out);
      auto [first, last] = std::ranges::unique(out);
      out.erase(first, last);
      return out;
    }
  }; // namespace core

  template<typename symbol>
  trie<symbol>
  make(core::dbs::database<symbol> auto const & db, std::size_t const & minsup) {
    trie<symbol> t(std::size(db));

    std::unordered_map<symbol, std::size_t> symbol_count;
    for (auto const & sequence : db) {
      for (symbol key : core::unique<symbol>(sequence)) {
        symbol_count[key] += 1;
      }
    }

    for (auto const [k, c] : symbol_count) {
      if (c >= minsup) {
        t.insert(k, make<symbol>(core::project<symbol>(db, k), minsup));
      }
    }

    return t;
  }
}; // namespace prefixspan
