#pragma once

#include <map>

namespace prefixspan {
  template<typename key_t>
  class trie {
    std::map<key_t, trie<key_t>> subtries;

    trie(trie<key_t> const & other) = delete;

    trie<key_t> & operator=(trie<key_t> const & other) = delete;

    public:

    trie() noexcept = default;
    
    trie(trie<key_t> && other) noexcept = default;

    trie<key_t> & operator=(trie<key_t> && other) noexcept = default;

    auto insert(key_t const & key, trie<key_t> && tries) {
      return this->subtries.emplace(key, std::move(tries));
    };

    auto insert(key_t const & key) {
      return this->subtries.emplace(key, std::move(trie<key_t>()));
    }

    bool contains(key_t const & key) const {
      return this->subtries.contains(key);
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

    const auto & unfix() const {
      return this->subtries;
    }

  };
}; // namespace prefixspan
