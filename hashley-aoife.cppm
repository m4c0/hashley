export module hashley:aoife;
import hai;

/// K->V map, using fixed-length array of hashes and a varray for bucket
/// list. Implementation copied from siobhan.
///
/// This class fulfills some requirements at the time of writing:
/// * Simplest implementation possible
/// * Small number of keys stored
namespace hashley {
  export template<typename K, typename V> class aoife {
    struct invalid_key {};
  
    struct pair {
      K key;
      V value;
    };
    using bucket = hai::varray<pair>;
  
    hai::array<bucket> m_data;
  
    [[nodiscard]] constexpr auto index_of(K key) const {
      return key % m_data.size();
    }
  
  public:
    explicit constexpr aoife(unsigned bucket_count) : m_data{bucket_count} {}
  
    [[nodiscard]] constexpr const auto &operator[](K key) const {
      for (auto &p : m_data[index_of(key)]) {
        if (p.key == key)
          return p.value;
      }
      throw invalid_key{};
    }
    [[nodiscard]] constexpr auto &operator[](K key) noexcept {
      auto &bkt = m_data[index_of(key)];
      for (auto &p : bkt) {
        if (p.key == key)
          return p.value;
      }
      bkt.push_back_doubling(pair{key, {}});
      return bkt[bkt.size() - 1].value;
    }
  
    [[nodiscard]] constexpr bool has(K key) const {
      for (auto &p : m_data[index_of(key)]) {
        if (p.key == key)
          return true;
      }
      return false;
    }
    constexpr void remove(K key) {
      auto &bkt = m_data[index_of(key)];
      for (auto &p : bkt) {
        if (p.key == key) {
          p = bkt[bkt.size() - 1];
          bkt.pop_back();
          return;
        }
      }
    }
  
    // Planned, but not implemented
    // [[nodiscard]] constexpr auto begin() const { return nullptr; }
    // [[nodiscard]] constexpr auto end() const { return nullptr; }
    // [[nodiscard]] constexpr auto begin() { return nullptr; }
    // [[nodiscard]] constexpr auto end() { return nullptr; }
  };
}

constexpr const auto fail = [] -> bool { throw 0; };

static_assert([] {
  hashley::aoife<int, int> m{3};
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
static_assert([] {
  hashley::aoife<int, int> m{3};
  for (auto i = 0; i < 100; i++) {
    m[i] = i * 100;
  }

  for (auto i = 0; i < 100; i++) {
    m.has(i) || fail();
  }
  (m[69] == 6900) || fail();

  m.remove(69);
  m.has(69) && fail();

  for (auto i = 0; i < 100; i++) {
    if (i == 69)
      continue;

    m.has(i) || fail();
  }
  return true;
}());
