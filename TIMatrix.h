//
// Created by cmk91 on 5/26/21.
//

#ifndef TIMATRIX_TIMATRIX_H
#define TIMATRIX_TIMATRIX_H
/**
 * Typed Index Matrix allows you to create a type for each index in your matrix, giving you some type safety so that
 * you don't mix up which iteration goes into which dimension.
 */
#include <memory>

template <typename indexty, typename returnty>
struct LayeredMatrix {
  std::unique_ptr<returnty> mat;
  returnty& operator[](indexty idx) {
    return mat.get()[idx.getN()];
  }
protected:
  void set_mat(returnty *a) {
    mat.reset(a); // = std::make_unique<returnty>(a);
  }
  returnty& get(indexty idx) {
    return mat.get()[int(idx)];
  }
};

struct TopLevelTraits {
  TopLevelTraits(const TopLevelTraits &_) = delete;
  TopLevelTraits() = default;
};

template <typename ty, typename idx1>
struct oneDimMat : LayeredMatrix<idx1, ty> {
  void init(idx1 a) {
    this->set_mat(new ty[int(a)]);
  }
};

template <typename ty, typename idx1, typename idx2>
struct twoDimMat : LayeredMatrix<idx1, oneDimMat<ty, idx2>> {
  void init(idx1 a, idx2 b) {
    this->set_mat(new oneDimMat<ty, idx2>[int(a)]);
    for (idx1 i = 0; i < a; ++i) {
      this->get(i).init(b);
    }
  }
};


template <typename ty, typename idx1, typename idx2, typename idx3>
struct threeDimMat : LayeredMatrix<idx1, twoDimMat<ty, idx2, idx3>> {
  void init(idx1 a, idx2 b, idx3 c) {
    this->set_mat(new twoDimMat<ty, idx2, idx3>[int(a)]);
    for (idx1 i = 0; i < a; ++i) {
      this->get(i).init(b, c);
    }
  }
};

template <typename ty, typename idx1, typename idx2, typename idx3, typename idx4>
struct fourDimMat : LayeredMatrix<idx1, threeDimMat<ty, idx2, idx3, idx4>> {
  void init(idx1 a, idx2 b, idx3 c, idx4 d) {
    this->set_mat(new threeDimMat<ty, idx2, idx3, idx4>[int(a)]);
    for (idx1 i = 0; i < a; ++i) {
      get(i).init(b, c, d);
    }
  }
};

#define declare_four_dim_mat(name, type, idx1, idx2, idx3, idx4) \
struct name : fourDimMat<type, idx1, idx2, idx3, idx4>, TopLevelTraits { \
name(idx1 i1, idx2 i2, idx3 i3, idx4 i4) { \
this->init(i1, i2, i3, i4) \
}\
};
#define declare_three_dim_mat(name, type, idx1, idx2, idx3, idx4) \
struct name : threeDimMat<type, idx1, idx2, idx3>, TopLevelTraits { \
name(idx1 i1, idx2 i2, idx3 i3) { \
this->init(i1, i2, i3) \
}\
};
#define declare_two_dim_mat(name, type, idx1, idx2) \
struct name : twoDimMat<type, idx1, idx2>, TopLevelTraits { \
name(idx1 i1, idx2 i2) { \
this->init(i1, i2) \
}\
};
#define declare_one_dim_mat(name, type, idx1) \
struct name : oneDimMat<type, idx1>, TopLevelTraits { \
name(idx1 i1) { \
this->init(i1) \
}\
};

#endif //TIMATRIX_TIMATRIX_H
