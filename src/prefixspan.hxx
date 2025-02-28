#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace prefixspan {
  namespace core {
    namespace ranges = std::ranges;

    template<typename type>
    using vector = std::vector<type>;

    namespace databases {

      template<typename type>
      concept database =
        ranges::range<type> && ranges::forward_range<ranges::range_value_t<type>>;

      template<database db_type>
      using range_t = ranges::range_value_t<db_type>;

      template<database db_type>
      using value_t = ranges::range_value_t<range_t<db_type>>;

      template<database db_type>
      using iterator_t = ranges::iterator_t<range_t<db_type>>;

      template<database db_type>
      using const_iterator_t = decltype(ranges::cbegin(std::declval<range_t<db_type> &>()));
    } // namespace databases

    template<databases::database db_type>
    auto project(db_type const & db, databases::value_t<db_type> const & key) {
      using subrange = ranges::subrange<databases::const_iterator_t<db_type>>;
      std::vector<subrange> out;
      for (databases::range_t<db_type> const & seq : db) {
        auto it = ranges::find(seq, key);
        if (it != seq.end()) {
          out.emplace_back(++it, seq.end());
        }
      }
      return out;
    };

    template<ranges::range seq_type>
    auto unique(seq_type const & seq) {
      std::vector<ranges::range_value_t<seq_type>> out(seq.begin(), seq.end());
      ranges::sort(out);
      auto [first, last] = ranges::unique(out);
      out.erase(first, last);
      return out;
    }
  }; // namespace core

  using namespace core;

  template<typename symbol>
  class prefixspan {
    using index = std::unordered_map<symbol, prefixspan<symbol>>;
    using iterator = typename index::iterator;
    using const_iterator = typename index::const_iterator;

    std::size_t m_count = 0;
    index unfixed;

    public:

    prefixspan() = default;

    prefixspan(prefixspan<symbol> const & other) noexcept = default;

    prefixspan(prefixspan<symbol> && other) noexcept = default;

    explicit prefixspan<symbol>(std::size_t const & count) noexcept : m_count(count){};

    template<databases::database db_type = std::initializer_list<std::initializer_list<symbol>>>
    prefixspan(db_type const & db, std::size_t const & minsup) : m_count(std::size(db)) {
      std::unordered_map<symbol, std::size_t> symbol_count;
      for (auto const & sequence : db) {
        for (symbol key : unique<databases::range_t<db_type>>(sequence)) {
          symbol_count[key] += 1;
        }
      }

      #pragma omp parallel
      #pragma omp single
      for (auto const entry : symbol_count) {
        symbol key = entry.first;
        std::size_t count = entry.second;
        if (count >= minsup) {
          #pragma omp task
          {
            auto projected_prefixspan = prefixspan<symbol>(project(db, key), minsup);
            #pragma omp critical
            {
              auto [it, inserted] = this->unfixed.try_emplace(key, std::move(projected_prefixspan));
              assert(inserted);
            }
          }
        }
      }
    }

    prefixspan<symbol> & operator=(prefixspan<symbol> const & other) noexcept = default;

    prefixspan<symbol> & operator=(prefixspan<symbol> && other) noexcept = default;

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

    std::unordered_map<symbol, prefixspan<symbol>> const &
    unfix() const noexcept {
      return unfixed;
    };
  };

}; // namespace prefixspan
