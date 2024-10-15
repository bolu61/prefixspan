from prefixspan import make_trie
import numpy as np


def test_make_trie_empty():
    make_trie([], 0)

def test_make_trie_sample():
    db = [
        np.array([0,1,2], np.uint64),
        np.array([0,2,2], np.uint64),
        np.array([0,1,1], np.uint64)
    ]
    t = make_trie(db, 2)
    assert repr(t) == "<prefixspan.trie 0,1,;2,;;1,;2,;>"
    assert t.count == 3
    assert [k for k, _ in t] == [0, 1, 2]
    assert [*t.keys] == [0, 1, 2]
    assert sorted(t.keys, key=lambda k: t[k].count) == [1, 2, 0]