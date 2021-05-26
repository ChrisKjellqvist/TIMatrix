//
// Created by cmk91 on 5/26/21.
//

#ifndef TIMATRIX_TIINDEX_H
#define TIMATRIX_TIINDEX_H
#define IDX_STORAGE_TY int

template<typename ty>
class index {
  IDX_STORAGE_TY n;
public:
  index& operator ++() {
    n++;
    return *this;
  }

  index& operator +=(int d) {
    n = n + d;
    return *this;
  }

  index operator + (const int d) const {
    return index(n + d);
  }

  index operator - (const int d) const {
    return index(n - d);
  }

  operator ty() {
    return ty(n);
  }

  explicit operator int() const {
    return n;
  }

  bool operator < (const index &rhs) const {
    return n < rhs.n;
  }

  bool operator > (const index &rhs) const {
    return n > rhs.n;
  }

  bool operator < (const int &rhs) const {
    return n < rhs;
  }

  bool operator > (const int &rhs) const {
    return n > rhs;
  }
  bool operator <= (const index &rhs) const {
    return n <= rhs.n;
  }

  bool operator >= (const index &rhs) const {
    return n >= rhs.n;
  }

  bool operator <= (const int &rhs) const {
    return n <= rhs;
  }

  bool operator >= (const int &rhs) const {
    return n >= rhs;
  }


  explicit index(int d) {
    n = d;
  }

  int getN() const {
    return n;
  }
};

#define indexify(tyname) struct tyname : index<tyname> {tyname (int d) : index(d) {}}

#endif //TIMATRIX_TIINDEX_H
