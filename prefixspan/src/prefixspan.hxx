#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <initializer_list>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace prefixspan {
  namespace core {
    template<typename sequence>
    using sequence_value_type = std::ranges::range_value_t<sequence>;

    template<typename sequence>
    using sequence_iterator_type = std::ranges::iterator_t<sequence>;

    template<typename sequence, typename symbol>
    concept sequence_type = std::ranges::random_access_range<sequence> &&
      std::ranges::sized_range<sequence> &&
      std::convertible_to<sequence_value_type<sequence>, symbol>;

    template<typename database, typename symbol>
    concept database_type =
      sequence_type<database, sequence_value_type<database>> &&
      sequence_type<sequence_value_type<database>, symbol>;

    template<typename symbol, database_type<symbol> database>
    auto project(database const & db, symbol const & key) {
      using iterator =
        sequence_iterator_type<sequence_value_type<database> const>;
      using subrange = std::ranges::subrange<iterator>;
      std::vector<subrange> out;
      for (sequence_type<symbol> auto const & seq : db) {
        iterator it = std::ranges::find(seq, key);
        if (it != seq.end()) {
          out.push_back(std::ranges::subrange(++it, seq.end()));
        }
      }
      return out;
    };

    template<typename symbol, core::sequence_type<symbol> sequence>
    std::vector<symbol> unique(sequence const & seq) {
      std::vector<symbol> out(std::ranges::begin(seq), std::ranges::end(seq));
      std::ranges::sort(out);
      auto [first, last] = std::ranges::unique(out);
      out.erase(first, last);
      return out;
    }
  }; // namespace core

  template<typename symbol>
  class prefixspan {
    std::size_t m_count = 0;
    std::unordered_map<symbol, prefixspan<symbol>> unfixed;
    using index = std::unordered_map<symbol, prefixspan<symbol>>;
    using iterator = typename index::iterator;
    using const_iterator = typename index::const_iterator;

    public:

    prefixspan() = default;

    prefixspan(prefixspan<symbol> const & other) noexcept = default;

    prefixspan(prefixspan<symbol> && other) noexcept = default;

    explicit prefixspan<symbol>(std::size_t const & count) noexcept :
      m_count(count){};

    template<
      core::database_type<symbol> database =
        std::initializer_list<std::initializer_list<symbol>>>
    prefixspan(database const & db, std::size_t const & minsup) :
      m_count(std::size(db)) {
      std::unordered_map<symbol, std::size_t> symbol_count;
      for (auto const & sequence : db) {
        for (symbol key : core::unique<symbol>(sequence)) {
          symbol_count[key] += 1;
        }
      }

      for (auto const [k, c] : symbol_count) {
        if (c >= minsup) {
          this->insert(k, trie<symbol>(core::project<symbol>(db, k), minsup));
        }
      }
    }

    prefixspan<symbol> & operator=(prefixspan<symbol> const & other) noexcept = default;

    prefixspan<symbol> & operator=(prefixspan<symbol> && other) noexcept = default;

    template<typename... arg_types>
    iterator insert(symbol const & key, arg_types &&... args) {
      auto [it, inserted] =
        unfixed.try_emplace(key, std::forward<arg_types>(args)...);
      if (inserted) {
        return it;
      }
      for (auto && [k, next] : prefixspan<symbol>(std::forward<arg_types>(args)...)) {
        it->second.insert(k, std::move(next));
      }
      return it;
    };

    void insert() {
      insert(1);
    };

    void insert(std::size_t const & count) {
      assert(count > 0);
      m_count += count;
    };

    prefixspan<symbol> & at(symbol const & key) {
      return unfixed.at(key);
    }

    prefixspan<symbol> const & at(symbol const & key) const {
      return unfixed.at(key);
    };

    prefixspan<symbol> & operator[](symbol const & key) {
      return unfixed[key];
    };

    bool contains(symbol const & key) const {
      return unfixed.contains(key);
    };

    std::size_t count() const noexcept {
      return m_count;
    }

    iterator begin() {
      return unfixed.begin();
    }

    const_iterator begin() const {
      return unfixed.begin();
    }

    iterator end() {
      return unfixed.end();
    }

    const_iterator end() const {
      return unfixed.end();
    }

    std::unordered_map<symbol, prefixspan<symbol>> & unfix() noexcept {
      return unfixed;
    };

    std::unordered_map<symbol, prefixspan<symbol>> const & unfix() const noexcept {
      return unfixed;
    };
  };

}; // namespace prefixspan
