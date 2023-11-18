#pragma once

#include <memory>

namespace prefixspan {
  template<typename T>
  using ref = std::shared_ptr<T>;
}; // namespace prefixspan
