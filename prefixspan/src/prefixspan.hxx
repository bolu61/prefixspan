#pragma once

#include "trie.hxx"

#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace prefixspan {
  namespace core {
    namespace dbs {
      template<typename sequence>
      using value = std::ranges::range_value_t<sequence>;

      template<typename sequence>
      using iterator = std::ranges::iterator_t<sequence>;

      template<typename sequence, typename symbol>
      concept sequence_type = std::ranges::random_access_range<sequence> &&
        std::ranges::sized_range<sequence> &&
        std::same_as<value<sequence>, symbol>;

      template<typename database, typename symbol>
      concept database_type = sequence_type<database, value<database>> &&
        sequence_type<value<database>, symbol>;
    } // namespace dbs

    template<typename symbol, dbs::database_type<symbol> database>
    auto project(database const & db, symbol const & key) {
      using iterator = dbs::iterator<const dbs::value<database>>;
      using subrange = std::ranges::subrange<iterator>;
      std::vector<subrange> out;
      for (dbs::sequence_type<symbol> auto const & seq : db) {
        iterator it = std::ranges::find(seq, key);
        if (it != seq.end()) {
          out.push_back(std::ranges::subrange(++it, seq.end()));
        }
      }
      return out;
    };

    template<typename symbol, dbs::sequence_type<symbol> sequence>
    std::vector<symbol> unique(sequence const & seq) {
      std::vector<symbol> out(std::ranges::begin(seq), std::ranges::end(seq));
      std::ranges::sort(out);
      auto [first, last] = std::ranges::unique(out);
      out.erase(first, last);
      return out;
    }
  }; // namespace core

  template<typename symbol, core::dbs::database_type<symbol> database>
  trie<symbol> make_trie(database const & db, std::size_t const & minsup) {
    trie<symbol> t(std::size(db));

    std::unordered_map<symbol, std::size_t> symbol_count;
    for (auto const & sequence : db) {
      for (symbol key : core::unique<symbol>(sequence)) {
        symbol_count[key] += 1;
      }
    }

    for (auto const [k, c] : symbol_count) {
      if (c >= minsup) {
        t.insert(k, make_trie<symbol>(core::project<symbol>(db, k), minsup));
      }
    }

    return t;
  }
}; // namespace prefixspan
