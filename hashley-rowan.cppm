export module hashley:rowan;
import hai;
import jute;

/// cstr->uint map, using a prefix tree ("trie").
///
/// This class fulfills some requirements at the time of writing:
/// * No template instantiation on client side (avoid weird clang-16 bugs)
/// * Own memory for string keys
/// * Main usage as a way of storing a unique list of strings
namespace hashley {
export class rowan {
  struct node {
    hai::array<hai::uptr<node>> children{256};
    unsigned value{};
  };
  node m_root{};

  constexpr unsigned recurse(jute::view key, const node * n) const {
    if (key.size() == 0) return n->value;

    auto &c = n->children[key[0]];
    if (!c) return {};

    return recurse(key.subview(1).after, &*c);
  }

  constexpr unsigned &recurse(jute::view key, node * n) {
    if (key.size() == 0) return n->value;

    auto &c = n->children[key[0]];
    if (!c) c.reset(new node{});

    return recurse(key.subview(1).after, &*c);
  }

public:
  constexpr unsigned &operator[](jute::view key) {
    return recurse(key, &m_root);
  }
  constexpr unsigned operator[](jute::view key) const {
    return recurse(key, &m_root);
  }
};
} // namespace hashley

static_assert([] {
  constexpr const auto fail = [] -> bool { throw 0; };

  hashley::rowan t{};
  t["A"] = 15;
  t["i"] = 11;
  t["in"] = 5;
  t["inn"] = 9;
  t["ten"] = 12;
  t["ted"] = 4;
  t["tea"] = 3;
  t["to"] = 7;

  (t["t"] == 0) || fail();
  (t["te"] == 0) || fail();
  (t["ted"] == 4) || fail();
  (t["tea"] == 3) || fail();

  const auto &tt = t;
  (tt["t"] == 0) || fail();
  (tt["te"] == 0) || fail();
  (tt["ted"] == 4) || fail();
  (tt["teresa"] == 0) || fail();

  return true;
}());
