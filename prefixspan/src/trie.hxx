#pragma once

#include <memory>
#include <ranges>
#include <unordered_map>

namespace prefixspan {
  template<typename key_t>
  class trie {
    std::size_t size;
    std::unordered_map<key_t, trie<key_t>> unfixed;

    public:

    trie() noexcept {
      this->size = 0;
    };

    auto insert(key_t const & key, trie<key_t> && tries) {
      return this->unfixed.emplace(key, std::move(tries));
    };

    auto insert(key_t const & key) {
      return this->unfixed.emplace(key, std::move(trie<key_t>()));
    }

    bool contains(key_t const & key) const {
      return this->unfixed.contains(key);
    }

    std::map<key_t, trie<key_t>> & unfix() noexcept {
      return this->unfixed;
    }

    std::map<key_t, trie<key_t>> const & unfix() const noexcept {
      return this->unfixed;
    }

    auto & at(key_t const & key) {
      return this->unfixed.at(key);
    }

    auto const & at(key_t const & key) const {
      return this->unfixed.at(key);
    }

    auto & operator[](key_t const & key) noexcept {
      return this->unfixed[key];
    }

    auto const & operator[](key_t const & key) const noexcept {
      return this->unfixed[key];
    }

    bool empty() const noexcept {
      return this->unfixed.empty();
    }
  };
}; // namespace prefixspan
