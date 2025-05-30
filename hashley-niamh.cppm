export module hashley:niamh;
import hai;
import jute;
import traits;

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
  export class niamh {
    struct pair {
      hai::cstr key;
      unsigned value;
    };
    using bucket = hai::varray<pair>;

    hai::array<bucket> m_data;

    [[nodiscard]] static constexpr unsigned long djb2(jute::view str) {
      unsigned long hash = 5381;
      for (int c : str) hash = hash * 33 ^ c;
      return hash;
    }

    [[nodiscard]] constexpr auto index_of(jute::view key) const { return djb2(key) % m_data.size(); }

  public:
    explicit constexpr niamh(unsigned bucket_count) : m_data { bucket_count } {}

    [[nodiscard]] constexpr const auto operator[](jute::view key) const {
      for (auto & p : m_data[index_of(key)])
        if (p.key == key) return p.value;
      return 0U;
    }
    [[nodiscard]] constexpr auto & operator[](jute::view key) {
      auto & bkt = m_data[index_of(key)];
      for (auto & p : bkt)
        if (p.key == key) return p.value;
      bkt.push_back_doubling(pair { key.cstr(), {} });
      return bkt[bkt.size() - 1].value;
    }

    [[nodiscard]] constexpr bool has(jute::view key) const {
      for (auto & p : m_data[index_of(key)])
        if (p.key == key) return true;
      return false;
    }
    constexpr void remove(jute::view key) {
      auto & bkt = m_data[index_of(key)];
      for (auto & p : bkt) {
        if (p.key == key) {
          p = traits::move(bkt[bkt.size() - 1]);
          bkt.pop_back();
          return;
        }
      }
    }
  };
} // namespace hashley

constexpr const auto fail = [] -> bool { throw 0; };

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
