#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include "prefixspan.hxx"

NB_MODULE(prefixspan, m) {
    nanobind::bind_vector<std::vector<unsigned int>>(m, "int_vector");
    m.def("build", &prefixspan::build<std::vector<unsigned int>>);
}
