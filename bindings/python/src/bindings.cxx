#include "prefixspan.hxx"

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
using sequence = std::vector<data>;
using database = std::vector<sequence>;

std::string stringify(ps::prefixspan<data> const & t) {
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
    "prefixspan_from_buffer",
    [](std::vector<nb::ndarray<data>> const & db, std::size_t const & minsup) {
      return ps::prefixspan<data>(
        db | std::views::transform([](nb::ndarray<data> const & s) {
          return std::ranges::subrange(s.data(), s.data() + s.size());
        }),
        minsup
      );
    },
    nb::arg("db"),
    nb::arg("minsup"),
    nb::rv_policy::move
  );

  nb::class_<ps::prefixspan<data>>(m, "prefixspan")
    .def(
      nb::init<database const &, std::size_t>(),
      nb::arg("db"),
      nb::arg("minsup")
    )
    .def(
      "__contains__",
      [](ps::prefixspan<data> const & t, data const & key) {
        return t.contains(key);
      }
    )
    .def(
      "__getitem__",
      [](ps::prefixspan<data> const & t, data const & key) {
        return t.at(key);
      },
      nb::rv_policy::reference_internal,
      nb::keep_alive<0, 1>()
    )
    .def_prop_ro(
      "count",
      [](ps::prefixspan<data> const & t) { return t.count(); },
      nb::rv_policy::reference_internal
    )
    .def(
      "__iter__",
      [](ps::prefixspan<data> const & t) {
        return nb::make_iterator(
          nb::type<ps::prefixspan<data>>(),
          "iterator",
          t.begin(),
          t.end()
        );
      },
      nb::keep_alive<0, 1>()
    )
    .def_prop_ro(
      "keys",
      [](ps::prefixspan<data> const & t) {
        auto it = t |
          std::ranges::views::transform([](auto & item) { return item.first; });
        return nb::make_iterator(
          nb::type<ps::prefixspan<data>>(),
          "key_iterator",
          it.begin(),
          it.end()
        );
      },
      nb::keep_alive<0, 1>()
    )
    .def("__repr__", [](ps::prefixspan<data> const & t) {
      return "<prefixspan " + stringify(t) + ">";
    });
}
