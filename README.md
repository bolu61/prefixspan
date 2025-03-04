# prefixspan

C++20 implementation of PrefixSpan pattern mining algorithm.

## Quick Start

```c++
#include <prefixspan.hxx>
#include <vector>

namespace ps = prefixspan;
using vec = std::vector;

int main() {
  vec<vec<int>> sequences {
    {0, 1, 2},
    {0, 2, 2},
    {0, 1, 1},
  };

  ps::prefixspan<int> pattern_tree(sequences, minsup=2); 
}
```

## Python Bindings

```sh
pip install git+https://github.com/bolu61/prefixspan@main#subdirectory=bindings/python
```

```python
from prefixspan import prefixspan
sequences = [
  [0, 1, 2],
  [0, 2, 2],
  [0, 1, 1],
]

pattern_tree = prefixspan(sequences, minsup=2)
assert repr(pattern_tree) == '<prefixspan 0,1,;2,;;1,;2,;>'
```
