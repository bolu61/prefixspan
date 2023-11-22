#include "prefixspan.hxx"
#include "trie.hxx"

#include <cstddef>
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/pair.h>
#include <nanobind/make_iterator.h>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

namespace nb = nanobind;
namespace ps = prefixspan;

  using data_t = std::size_t;
  using sequence_t = std::vector<data_t>;
  using database_t = std::vector<sequence_t>;


std::string stringify(prefixspan::trie<data_t> const & t) {
  if (t.empty()) {
    return "";
  }
  std::vector<std::string> strings;
  for (auto const & [symbol, next] : t.unfix()) {
    strings.emplace_back(std::to_string(symbol) + ',' + stringify(next) + ";");
  }
  std::ostringstream out;
  std::copy(
    strings.begin(),
    strings.end(),
    std::ostream_iterator<std::string>(out, "")
  );
  return out.str();
}

NB_MODULE(prefixspan_python_ext, m) {
  nanobind::class_<prefixspan::trie<data_t>>(m, "trie")
    .def(
      "__getitem__",
      [](prefixspan::trie<data_t> const & t, data_t const & key) {
        return t.at(key);
      }
    )
    .def("empty", &prefixspan::trie<data_t>::empty)
    .def(
      "__iter__",
      [](prefixspan::trie<data_t> const & t) {
        auto iterator = t.unfix();
        return nanobind::make_iterator(
          nanobind::type<prefixspan::trie<data_t>>(),
          "iterator",
          iterator.begin(),
          iterator.end()
        );
      },
      nanobind::keep_alive<0, 1>()
    )
    .def("__repr__", &stringify);

  m.def(
    "make",
    [](
      database_t const & db,
      std::size_t const & minsup
    ) { return prefixspan::make<data_t, database_t>(db, minsup); }
  );
}
