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
  if (t.size() == 0) {
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

TEST_CASE("simple input", "[prefixspan]") {
  database sample_db{{0, 1, 2}, {0, 2}, {0, 2}, {0, 1}};
  auto t = prefixspan::make<symbol>(sample_db, 3);
  REQUIRE(stringify(t) == "0,2,;;2,;");
  REQUIRE(t[0].size() == 4);
  REQUIRE(t[1].size() == 0);
  REQUIRE(t[2].size() == 3);
  REQUIRE(t[0][0].size() == 0);
  REQUIRE(t[0][1].size() == 0);
  REQUIRE(t[0][2].size() == 3);
}

TEST_CASE("duplicate symbol in sequence", "[prefixspan]") {
  database sample_db{{0, 1, 2, 1}};
  auto t = prefixspan::make<symbol>(sample_db, 0);
  REQUIRE(t[1].size() == 1);
}
