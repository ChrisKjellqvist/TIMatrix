//
// Created by cmk91 on 5/26/21.
//

#ifndef TIMATRIX_TIINDEX_H
#define TIMATRIX_TIINDEX_H

#include <iterator>

#define IDX_STORAGE_TY int

namespace std {
  struct typed_index_tag {};
}

template<typename idx2, typename idx1>
idx2 index_cast(const idx1 &I) {
  return idx2(IDX_STORAGE_TY(I));
}

template<typename ty>
class free_index {
  IDX_STORAGE_TY n;
public:
  typedef free_index difference_type;
  typedef ty value_type;
  typedef std::typed_index_tag iterator_category;

  free_index &operator++() {
    n++;
    return *this;
  }

  free_index operator++(int) {
    auto ret = *this;
    n++;
    return ret;
  }

  free_index &operator+=(difference_type d) {
    n = n + d.n;
    return *this;
  }

  free_index &operator-=(difference_type d) {
    n = n - d.n;
    return *this;
  }

  free_index operator+(const difference_type d) const {
    return free_index(n + d.n);
  }

  free_index operator-(const difference_type d) const {
    return free_index(n - d.n);
  }

  free_index operator-() const {
    return free_index(-1 * n);
  }

  operator ty() {
    return ty(n);
  }

  explicit operator int() const {
    return n;
  }

  explicit operator size_t() const {
    return static_cast<size_t>(n);
  }

  bool operator<(const free_index &rhs) const {
    return n < rhs.n;
  }

  bool operator>(const free_index &rhs) const {
    return n > rhs.n;
  }

  bool operator<(const int &rhs) const {
    return n < rhs;
  }

  bool operator>(const int &rhs) const {
    return n > rhs;
  }

  bool operator<=(const free_index &rhs) const {
    return n <= rhs.n;
  }

  bool operator>=(const free_index &rhs) const {
    return n >= rhs.n;
  }

  bool operator<=(const int &rhs) const {
    return n <= rhs;
  }

  bool operator>=(const int &rhs) const {
    return n >= rhs;
  }

  bool operator==(const free_index rhs) const {
    return n == rhs.n;
  }

  friend bool operator<(int lhs, const free_index &rhs) {
    return lhs < rhs.n;
  }

  friend bool operator>(int lhs, const free_index &rhs) {
    return lhs > rhs.n;
  }

  friend bool operator<=(int lhs, const free_index &rhs) {
    return lhs <= rhs.n;
  }

  friend bool operator>=(int lhs, const free_index &rhs) {
    return lhs >= rhs.n;
  }

  explicit free_index(int32_t d) {
    n = d;
  }

  [[nodiscard]] int getN() const {
    return n;
  }
};

// purposefully make conversion operators implicit
struct int_index : free_index<int_index> {
  int_index(int d) : free_index<int_index>(d) {}
  operator int() const {
    return getN();
  }
  operator size_t() const {
    return static_cast<size_t>(getN());
  }
};


#define declare_free_index(tyname) struct tyname : free_index<tyname> { explicit tyname (IDX_STORAGE_TY d) : free_index(d) {}}; \
tyname operator "" _ ## tyname (unsigned long long value);

#define add_UDL_idx(tyname) \
tyname operator "" _ ## tyname (unsigned long long value){ \
return tyname(static_cast<IDX_STORAGE_TY>(value)); \
}


#endif //TIMATRIX_TIINDEX_H
