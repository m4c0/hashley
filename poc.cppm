export module poc;

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

  constexpr void remove(unsigned key) {}

  [[nodiscard]] constexpr auto begin() const { return nullptr; }
  [[nodiscard]] constexpr auto end() const { return nullptr; }

  [[nodiscard]] constexpr auto begin() { return nullptr; }
  [[nodiscard]] constexpr auto end() { return nullptr; }
};

extern "C" int main() {}
