export module hashley:siobhan;

/// uint->uint map, using fixed-length array of hashes and a varray for bucket
/// list.
///
/// This class fulfills some requirements at the time of writing:
/// * No template instantiation on client side (avoid weird clang-16 bugs)
/// * Simplest implementation possible
/// * Small number of keys stored
class siobhan {
public:
  explicit constexpr siobhan(unsigned bucket_count) {}

  [[nodiscard]] constexpr const auto &operator[](unsigned key) const {
    return *(unsigned *)0;
  }
  [[nodiscard]] constexpr auto &operator[](unsigned key) {
    return *(unsigned *)0;
  }

  [[nodiscard]] constexpr bool has(unsigned key) const { return false; }
  constexpr void remove(unsigned key) {}

  // Planned, but not implemented
  // [[nodiscard]] constexpr auto begin() const { return nullptr; }
  // [[nodiscard]] constexpr auto end() const { return nullptr; }
  // [[nodiscard]] constexpr auto begin() { return nullptr; }
  // [[nodiscard]] constexpr auto end() { return nullptr; }
};

static_assert([] {
  constexpr const auto fail = [] -> bool { throw 0; };

  siobhan m{3};
  m[1] = 99;
  m[4] = 15;
  m[9] = 10;

  m.has(0) && fail();
  m.has(1) || fail();
  m.has(4) || fail();
  m.has(7) && fail();
  m.has(9) || fail();

  (m[0] == 0) || fail();
  (m[1] == 99) || fail();
  (m[4] == 15) || fail();
  (m[9] == 10) || fail();

  m.remove(1);

  m.has(9) || fail();
  (m[9] == 10) || fail();

  return true;
}());
