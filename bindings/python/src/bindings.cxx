#include "prefixspan.hxx"
#include "trie.hxx"

#include <cstddef>
#include <format>
#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

namespace nb = nanobind;
namespace ps = prefixspan;

using data = unsigned int;
using sequence = nb::ndarray<data>;
using database = std::vector<sequence>;

std::string stringify(ps::trie<data> const & t) {
  if (t.count() == 0) {
    return "";
  }
  std::vector<std::string> strings;
  for (auto const & [symbol, next] : t.unfix()) {
    strings.emplace_back(std::format("{},{};", symbol, stringify(next)));
  }

  return std::reduce(strings.begin(), strings.end());
}

NB_MODULE(prefixspan, m) {
  m.def(
    "make_trie",
    [](database const & db, std::size_t const & minsup) {
      return ps::make_trie<data>(
        db | std::views::transform([](sequence const & s) {
          return std::ranges::subrange(s.data(), s.data() + s.size());
        }),
        minsup
      );
    },
    nb::arg("db"),
    nb::arg("minsup"),
    nb::rv_policy::move
  );

  nb::class_<ps::trie<data>>(m, "trie")
    .def(
      "__contains__",
      [](ps::trie<data> const & t, data const & key) { return t.contains(key); }
    )
    .def(
      "__getitem__",
      [](ps::trie<data> const & t, data const & key) { return t.at(key); },
      nb::rv_policy::reference_internal,
      nb::keep_alive<0, 1>()
    )
    .def_prop_ro("count", [](ps::trie<data> const & t) { return t.count(); }, nb::rv_policy::reference_internal)
    .def(
      "__iter__",
      [](ps::trie<data> const & t) {
        return nb::make_iterator(
          nb::type<ps::trie<data>>(),
          "iterator",
          t.begin(),
          t.end()
        );
      },
      nb::keep_alive<0, 1>()
    )
    .def_prop_ro(
      "keys",
      [](ps::trie<data> const & t) {
        auto it = t |
          std::ranges::views::transform([](auto & item) { return item.first; });
        return nb::make_iterator(
          nb::type<ps::trie<data>>(),
          "key_iterator",
          it.begin(),
          it.end()
        );
      },
      nb::keep_alive<0, 1>()
    )
    .def("__repr__", [](ps::trie<data> const & t) {
      return "<prefixspan.trie " + stringify(t) + ">";
    });
}
