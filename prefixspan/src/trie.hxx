#pragma once

#include <cassert>
#include <unordered_map>
#include <utility>

namespace prefixspan {
  template<typename symbol>
  class trie {
    std::size_t m_count = 0;
    std::unordered_map<symbol, trie<symbol>> unfixed;
    using index = std::unordered_map<symbol, trie<symbol>>;
    using iterator = index::iterator;
    using const_iterator = index::const_iterator;

    public:

    trie<symbol>() = default;

    trie<symbol>(trie<symbol> const & other) noexcept = default;

    trie<symbol>(trie<symbol> && other) noexcept = default;

    explicit trie<symbol>(std::size_t const & count) noexcept : m_count(count){};

    trie<symbol> & operator=(trie<symbol> const & other) noexcept = default;

    trie<symbol> & operator=(trie<symbol> && other) noexcept = default;

    template<typename... arg_types>
    iterator insert(symbol const & key, arg_types&&... args) {
      auto [it, inserted] = unfixed.try_emplace(key, std::forward<arg_types>(args)...);
      if (inserted) {
        return it;
      }
      for (auto && [k, next] : trie<symbol>(std::forward<arg_types>(args)...)) {
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

    trie<symbol> & at(symbol const & key) {
      return unfixed.at(key);
    }

    trie<symbol> const & at(symbol const & key) const {
      return unfixed.at(key);
    };

    trie<symbol> & operator[](symbol const & key) {
      return unfixed[key];
    };

    bool contains(symbol const & key) const {
      return unfixed.contains(key);
    };

    std::size_t count() const noexcept {
      return m_count;
    }

    std::unordered_map<symbol, trie<symbol>>::iterator begin() {
      return unfixed.begin();
    }

    std::unordered_map<symbol, trie<symbol>>::const_iterator begin() const {
      return unfixed.begin();
    }

    std::unordered_map<symbol, trie<symbol>>::iterator end() {
      return unfixed.end();
    }

    std::unordered_map<symbol, trie<symbol>>::const_iterator end() const {
      return unfixed.end();
    }

    std::unordered_map<symbol, trie<symbol>> & unfix() noexcept {
      return unfixed;
    };

    std::unordered_map<symbol, trie<symbol>> const & unfix() const noexcept {
      return unfixed;
    };
  };
}; // namespace prefixspan
