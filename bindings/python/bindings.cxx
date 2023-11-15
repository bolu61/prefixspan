#include "prefixspan.hxx"

#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>

NB_MODULE(prefixspan, m) {
	nanobind::bind_vector<std::vector<unsigned int>>(m, "int_vector");
}
