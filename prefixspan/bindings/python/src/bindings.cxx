#include "prefixspan.hxx"
#include "trie.hxx"

#include <cstddef>
#include <format>
#include <iterator>
#include <memory>
#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

namespace nb = nanobind;
namespace ps = prefixspan;

using data_t = std::size_t;
using sequence_t = std::vector<data_t>;
using database_t = std::vector<sequence_t>;

using trie = std::shared_ptr<ps::trie<data_t> const>;

std::string stringify(ps::trie<data_t> const & t) {
  if (t.empty()) {
    return "";
  }
  std::vector<std::string> strings;
  for (auto const & [symbol, next] : t.unfix()) {
    strings.emplace_back(std::to_string(symbol) + "," + stringify(next) + ";");
  }

  return std::reduce(strings.begin(), strings.end());
}

NB_MODULE(prefixspan_python_ext, m) {
  nanobind::class_<trie>(m, "trie")
    .def(
      "__contains__",
      [](trie t, data_t const & key) { return t->contains(key); }
    )
    .def(
      "__getitem__",
      [](trie t, data_t const & key) { return trie(t, &t->at(key)); }
    )
    .def("empty", [](trie t) { return t->empty(); })
    .def(
      "__iter__",
      [](trie t) {
        auto const & iterator =
          t->unfix() | std::ranges::views::transform([&t](auto & item) {
            return std::pair<data_t, trie>(item.first, trie(t, &item.second));
          });
        return nanobind::make_iterator(
          nanobind::type<trie>(),
          "iterator",
          iterator.begin(),
          iterator.end()
        );
      },
      nanobind::keep_alive<0, 1>()
    )
    .def("__repr__", [](trie const & t) {
      return "<trie " + stringify(*t) + ">";
    });

  m.def("make", [](database_t const & db, std::size_t const & minsup) {
    return std::make_shared<const ps::trie<data_t>>(
      ps::make<data_t, database_t>(db, minsup)
    );
  });
}
