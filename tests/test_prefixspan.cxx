#include "prefixspan.hxx"

#include <random>
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace ps = prefixspan;

using symbol = unsigned int;

using database = std::vector<std::vector<symbol>>;

template class ps::prefixspan<symbol>;

template<typename symbol>
std::string stringify(ps::prefixspan<symbol> const & t) {
  if (t.count() == 0) {
    return "";
  }
  std::vector<symbol> keys;
  keys.reserve(t.unfix().size());
  for (auto const & it : t.unfix()) {
    keys.push_back(it.first);
  }
  std::sort(keys.begin(), keys.end());
  
  std::vector<std::string> strings;
  strings.reserve(keys.size());
  for (auto const & key : keys) {
    strings.emplace_back(std::to_string(key) + ',' + stringify(t.at(key)) + ';');
  }
  std::ostringstream out;
  std::ranges::copy(strings, std::ostream_iterator<std::string>(out, ""));
  return out.str();
};

std::vector<std::vector<symbol>> generate_large_db() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::poisson_distribution<> d(4);
  std::vector<std::vector<symbol>> db;
  db.reserve(10000);
  for (std::size_t i = 0; i < 10000; ++i) {
    auto & s = db.emplace_back(16);
    std::ranges::generate(s, [&]() { return d(gen); });
  }
  return db;
}

TEST_CASE("empty database", "[prefixspan]") {
  ps::prefixspan<symbol> t = ps::prefixspan<symbol>({}, 3);
  REQUIRE(stringify(t) == "");
  REQUIRE(t.count() == 0);
}

TEST_CASE("simple input", "[prefixspan]") {
  database sample_db{{0, 1, 2}, {0, 2, 2}, {0, 1, 1}};
  auto t = ps::prefixspan<symbol>(sample_db, 2);
  REQUIRE(stringify(t) == "0,1,;2,;;1,;2,;");
}

TEST_CASE("duplicate symbol in sequence", "[prefixspan]") {
  database sample_db{{0, 1, 2, 1}};
  auto t = ps::prefixspan<symbol>(sample_db, 0);
  REQUIRE(t[1].count() == 1);
}

TEST_CASE("large input", "[prefixspan]") {
  database sample_db = generate_large_db();
  auto t = ps::prefixspan<symbol>(sample_db, 100);
  REQUIRE(t.count() != 0);
}