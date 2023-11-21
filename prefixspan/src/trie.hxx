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
      return this->subtries.emplace(key);
    }
  };
}; // namespace prefixspan
