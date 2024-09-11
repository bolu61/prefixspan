#include "prefixspan.hxx"

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

template<typename symbol_t>
std::string stringify(prefixspan::trie<symbol_t> const & t) {
  if (t.empty()) {
    return "";
  }
  std::vector<std::string> strings;
  for (auto const & [symbol, next] : t.unfix()) {
    strings.emplace_back(std::to_string(symbol) + ',' + stringify(next) + ";");
  }
  std::ostringstream out;
  std::ranges::copy(
    strings.begin(),
    strings.end(),
    std::ostream_iterator<std::string>(out, "")
  );
  return out.str();
};

int main(int argc, char * argv[]) {
  std::vector<std::vector<unsigned int>> db{{0, 1, 2}, {0, 2}, {0, 2}, {0, 1}};
  auto a = prefixspan::prefixspan<unsigned int, std::vector<std::vector<unsigned int>>>(db, 0);
  std::cout << stringify(a) << std::endl;
}
 
