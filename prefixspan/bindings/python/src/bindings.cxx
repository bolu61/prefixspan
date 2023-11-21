#include "prefixspan.hxx"

#include <cstddef>
#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <ranges>
#include <vector>

NB_MODULE(prefixspan, m) {
  using data_t = std::size_t;
  using sequence_t = std::vector<data_t>;
  using database_t = std::vector<sequence_t>;

  nanobind::bind_vector<sequence_t>(m, "sequence");
  nanobind::bind_vector<database_t>(m, "database");

  m.def("make", [](database_t const & database) {
    return prefixspan::make<database_t, data_t>(database);
  });
}
