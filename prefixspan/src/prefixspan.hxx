#pragma once

#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <variant>

namespace prefixspan {
	template<typename node_t = std::monostate, typename edge_t = std::monostate>
	struct graph {
		using index_t = unsigned int;

		static bool
		insert(graph<node_t, edge_t> & g, index_t const & v, node_t value);

		static bool insert(
		  graph<node_t, edge_t> & g,
		  index_t const & u,
		  index_t const & v,
		  edge_t value
		);

		static std::vector<index_t>
		successors(graph<node_t, edge_t> const & g, index_t const & u);

		private:
		std::map<index_t, node_t> node;
		std::map<std::pair<index_t, index_t>, edge_t> edge;
		std::multimap<index_t, index_t> succ;
	};

	template<typename node_t = std::monostate, typename edge_t = std::monostate>
	struct tree {
		static auto
		rooted_at(graph<node_t, edge_t> & g, graph<>::index_t const & r);
		private:
		graph<node_t, edge_t>::index_t root;
		std::shared_ptr<graph<node_t, edge_t>> g;
	};

	template<typename T, typename R>
	requires std::ranges::random_access_range<R> &&
	  std::ranges::random_access_range<std::ranges::range_value_t<R>> &&
	  std::same_as<T, std::ranges::range_value_t<std::ranges::range_value_t<R>>>
	auto build(R const & sequences);

	template<typename T, typename S>
	requires std::ranges::random_access_range<S> &&
	  std::same_as<T, std::ranges::range_value_t<S>>
	auto sliding(S const & sequence, unsigned int width, unsigned int stride);

}; // namespace prefixspan

using namespace prefixspan;

template<typename node_t, typename edge_t>
bool graph<node_t, edge_t>::insert(
  graph<node_t, edge_t> & g,
  graph<>::index_t const & v,
  node_t value
) {
	const auto [it, success] = g.node.insert({ v, value });
	return success;
}

template<typename node_t, typename edge_t>
bool graph<node_t, edge_t>::insert(
  graph<node_t, edge_t> & g,
  graph<>::index_t const & u,
  graph<>::index_t const & v,
  edge_t value
) {
	const auto [it, success] = g.node.insert({ { u, v }, value });
	return success;
}

namespace prefixspan::core {
	template<typename T, typename R>
	requires std::ranges::random_access_range<R> &&
	  std::ranges::random_access_range<std::ranges::range_value_t<R>> &&
	  std::same_as<T, std::ranges::range_value_t<std::ranges::range_value_t<R>>>
	void recbuild(graph<unsigned int, T> g, R const & sequences);
} // namespace prefixspan::core

template<typename T, typename R>
requires std::ranges::random_access_range<R> &&
  std::ranges::random_access_range<std::ranges::range_value_t<R>> &&
  std::same_as<T, std::ranges::range_value_t<std::ranges::range_value_t<R>>>
auto build(R const & sequences) {
	auto alphabet = std::set<T>();
	for (auto s : sequences) {
		alphabet.insert(s.begin(), s.end());
	}

	auto g = graph<unsigned int, T>();
	graph<unsigned int, T>::insert(g, 0, 0);

	return g;
};
