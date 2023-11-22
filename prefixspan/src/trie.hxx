#pragma once

#include <map>
#include <ranges>

namespace prefixspan {
  template<typename key_t>
  class trie {
    std::map<key_t, trie<key_t>> subtries;

    public:

    auto insert(key_t const & key, trie<key_t> && tries) {
      return this->subtries.emplace(key, std::move(tries));
    };

    auto insert(key_t const & key) {
      return this->subtries.emplace(key, std::move(trie<key_t>()));
    }

    bool contains(key_t const & key) {
      return this->subtries.contains(key);
    }

    std::map<key_t, trie<key_t>> const & unfix() const {
      return this->subtries;
    }

    auto & at(key_t const & key) {
      return this->subtries.at(key);
    }

    auto const & at(key_t const & key) const {
      return this->subtries.at(key);
    }

    auto & operator[](key_t const & key) {
      return this->at(key);
    }

    auto const & operator[](key_t const & key) const {
      return this->at(key);
    }

    bool empty() const {
      return this->subtries.empty();
    }
  };
}; // namespace prefixspan
