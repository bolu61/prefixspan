#include "prefixspan.hxx"

#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using symbol = unsigned int;

using database = std::vector<std::vector<symbol>>;

template class prefixspan::trie<symbol>;

template<typename symbol>
std::string stringify(prefixspan::trie<symbol> const & t) {
  if (t.count() == 0) {
    return "";
  }
  std::vector<std::string> strings;
  for (auto const & [key, next] : t.unfix()) {
    strings.emplace_back(std::to_string(key) + ',' + stringify(next) + ';');
  }
  std::ostringstream out;
  std::ranges::copy(strings, std::ostream_iterator<std::string>(out, ""));
  return out.str();
};

TEST_CASE("empty database", "[prefixspan]") {
  prefixspan::trie<symbol> t = prefixspan::trie<symbol>({}, 3);
  REQUIRE(stringify(t) == "");
  REQUIRE(t.count() == 0);
}

TEST_CASE("simple input", "[prefixspan]") {
  database sample_db{{0, 1, 2}, {0, 2, 2}, {0, 1, 1}};
  auto t = prefixspan::trie<symbol>(sample_db, 2);
  REQUIRE(stringify(t) == "0,1,;2,;;1,;2,;");
}

TEST_CASE("duplicate symbol in sequence", "[prefixspan]") {
  database sample_db{{0, 1, 2, 1}};
  auto t = prefixspan::trie<symbol>(sample_db, 0);
  REQUIRE(t[1].count() == 1);
}
