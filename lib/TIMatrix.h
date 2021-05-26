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
#include <cassert>
#include "TIIndex.h"

template <typename indexty, typename returnty>
struct LayeredMatrix {
  std::unique_ptr<returnty[]> mat = nullptr;
  size_t mat_size;
  bool initialized = false;
  returnty& operator[](indexty idx) {
      return mat.get()[idx.getN()];
  }
  returnty* begin(){
      return mat.get();
  }
  returnty* end(){
      return mat.get() + mat_size;
  }
  ~LayeredMatrix(){
      assert(initialized);
  }
protected:
  template<typename ARRAY_TYPE>
  void set_mat(ARRAY_TYPE a, indexty size) {
    mat.reset(a);
    assert(!initialized);
    initialized = true;
    mat_size = static_cast<size_t>(size);
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
struct OneDimMat : LayeredMatrix<idx1, ty> {
  void init(idx1 a) {
    this->set_mat(new ty[int(a)], a);
  }
};

template <typename ty, typename idx1, typename idx2>
struct TwoDimMat : LayeredMatrix<idx1, OneDimMat<ty, idx2>> {
  void init(idx1 a, idx2 b) {
    this->set_mat(new OneDimMat<ty, idx2>[int(a)], a);
    for (idx1 i(0); i < a; ++i) {
      this->get(i).init(b);
    }
  }
};

template <typename ty, typename idx1, typename idx2, typename idx3>
struct ThreeDimMat : LayeredMatrix<idx1, TwoDimMat<ty, idx2, idx3>> {
  void init(idx1 a, idx2 b, idx3 c) {
    this->set_mat(new TwoDimMat<ty, idx2, idx3>[int(a)], a);
    for (idx1 i(0); i < a; ++i) {
      this->get(i).init(b, c);
    }
  }
};

template <typename ty, typename idx1, typename idx2, typename idx3, typename idx4>
struct FourDimMat : LayeredMatrix<idx1, ThreeDimMat<ty, idx2, idx3, idx4>> {
  void init(idx1 a, idx2 b, idx3 c, idx4 d) {
    this->set_mat(new ThreeDimMat<ty, idx2, idx3, idx4>[int(a)], a);
    for (idx1 i(0); i < a; ++i) {
      get(i).init(b, c, d);
    }
  }
};

#define declare_four_dim_mat(name, type, idx1, idx2, idx3, idx4) \
struct name : FourDimMat<type, idx1, idx2, idx3, idx4>, TopLevelTraits { \
name(idx1 i1, idx2 i2, idx3 i3, idx4 i4) { \
this->init(i1, i2, i3, i4) \
}\
};
#define declare_three_dim_mat(name, type, idx1, idx2, idx3, idx4) \
struct name : ThreeDimMat<type, idx1, idx2, idx3>, TopLevelTraits { \
name(idx1 i1, idx2 i2, idx3 i3) { \
this->init(i1, i2, i3) \
}\
};
#define declare_two_dim_mat(name, type, idx1, idx2) \
struct name : TwoDimMat<type, idx1, idx2>, TopLevelTraits { \
name(idx1 i1, idx2 i2) { \
this->init(i1, i2); \
}\
};
#define declare_one_dim_mat(name, type, idx1) \
struct name : OneDimMat<type, idx1>, TopLevelTraits { \
name(idx1 i1) { \
this->init(i1) \
}\
};

#endif //TIMATRIX_TIMATRIX_H
