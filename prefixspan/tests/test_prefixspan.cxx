#include "prefixspan.hxx"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using symbol = unsigned int;

using database = std::vector<std::vector<symbol>>;

template<typename symbol_t>
std::string stringify(prefixspan::trie<symbol_t> const & t) {
  if (t.empty()) {
    return "";
  }
  std::vector<std::string> strings;
  for (auto const & [symbol, next] : t.unfix()) {
    strings.emplace_back(std::to_string(symbol) + ',' + stringify(next) + ';');
  }
  std::ostringstream out;
  std::ranges::copy(strings, std::ostream_iterator<std::string>(out, ""));
  return out.str();
};

std::string run_prefixspan(database const & db, std::size_t const & minsup) {
  return stringify(
    prefixspan::make<unsigned int, std::vector<std::vector<unsigned int>>>(
      db,
      minsup
    )
  );
}

TEST_CASE("Run prefixspan on sample input.", "[prefixspan]") {
  std::vector<std::vector<unsigned int>>
    sample_db{{0, 1, 2}, {0, 2}, {0, 2}, {0, 1}};
  REQUIRE(run_prefixspan(sample_db, 3) == "0,2,;;2,;");
}
