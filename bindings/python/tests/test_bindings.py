from prefixspan import prefixspan


def test_make_trie_empty():
    prefixspan([], 0)


def test_make_trie_sample():
    db = [
        [0, 1, 2],
        [0, 2, 2],
        [0, 1, 1],
    ]
    t = prefixspan(db, 2)
    assert repr(t) == "<prefixspan.trie 0,1,;2,;;1,;2,;>"
    assert t.count == 3
    assert [k for k, _ in t] == [0, 1, 2]
    assert [*t.keys] == [0, 1, 2]
    assert sorted(t.keys, key=lambda k: t[k].count) == [1, 2, 0]
