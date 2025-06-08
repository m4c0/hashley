export module hashley:niamh;
import :fin;
import jute;

/// str->uint map, using fixed-length array of hashes and a varray for bucket
/// list.
///
/// Based on sioabhan in this same module.
///
/// This class fulfills some requirements at the time of writing:
/// * No template instantiation on client side (avoid weird clang-16 bugs)
/// * Simplest implementation possible
/// * Small number of keys stored
/// * Ownership of keys stored to avoid mutation after allocation
namespace hashley {
  export using niamh = fin<unsigned int>;
} // namespace hashley

static constexpr const auto fail = [] -> bool { throw 0; };

static_assert([] {
  using namespace jute::literals;

  hashley::niamh m { 3 };
  m["1"] = 99;
  m["4"] = 15;
  m["9"] = 10;

  m.has("0") && fail();
  m.has("1") || fail();
  m.has("4") || fail();
  m.has("7") && fail();
  m.has("9") || fail();

  (m["0"] == 0) || fail();
  (m["1"] == 99) || fail();
  (m["4"] == 15) || fail();
  (m["9"] == 10) || fail();

  m.remove("1");

  m.has("9") || fail();
  (m["9"] == 10) || fail();

  return true;
}());
