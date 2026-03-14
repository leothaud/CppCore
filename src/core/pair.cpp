export module core:pair;

namespace core {

export template <typename T1, typename T2> struct Pair {
  T1 first;
  T2 second;
};

}; // namespace core