//
// Created by martin on 2021-05-27.
//

#ifndef STATISTICAL_ROBUST_SLICES_H
#define STATISTICAL_ROBUST_SLICES_H

template<typename IT, typename INDEXABLE_RANGE, typename SELECTOR>
struct IteratorFilter {
  INDEXABLE_RANGE *container;
  SELECTOR selector;
  IT current;

  using size_type = typename INDEXABLE_RANGE::size_type;
  size_type index = size_type(0);

  // Do we do arithmetic as well? we probably should
  IteratorFilter operator++() {
    do {
      index++;
      current++;
    } while (!selector(index) && current != container->end());
    return *this;
  }

  IteratorFilter operator++(int) {
    return ++(*this);
  }

  decltype(*(IT())) &operator*() {
    return *current;
  }

  // Hack you say? We can't know what out of range is or can we?
  bool operator!=(IteratorFilter rhs) {
    //return index < rhs.index;
    return current != container->end();
  }

  bool operator==(IteratorFilter rhs) {
    return current == rhs.current;
  }
};

template <typename idxtupty, typename matty>
struct idx_into_struct {
  constexpr static int max_depth() {return std::tuple_size<idxtupty>{};}
  template <size_t i>
  static decltype(&matty::SubLayerType) index(idxtupty &tup, matty &mat) {
    return idx_into_struct<idxtupty, decltype(matty::SubLayerType)>::index<i+1>(tup, mat[std::get<i>(tup)]);
  }
  template <>
  static decltype(&matty::SubLayerType) index<max_depth()>(idxtupty &tup, matty &mat) {
    return mat[std::get<max_depth()-1>(tup)];
  }
};

// TODO - come up with a static type check for container_ty index types and enumerator_ty's return type
template<typename IT, typename CONTAINER_TY, typename ENUMERATOR_TY>
struct IteratorEnumerator {
  CONTAINER_TY *container;
  IT current;
  ENUMERATOR_TY enumerator;

private:
  typedef decltype(std::tuple_element<1, ENUMERATOR_TY()>::type) indextypes;
  size_t enumerator_dims = std::tuple_size<indextypes>{};
public:

  IteratorEnumerator operator++() {
    current = enumerator();
  }

  IteratorEnumerator operator++(int) {
    return ++(*this);
  }


  decltype(*(IT())) &operator*() {
    return idx_into_struct<indextypes, CONTAINER_TY>::index<0>(current, *container);
  }

  bool operator!=(IteratorEnumerator rhs) {
    //return index < rhs.index;
    return current != container->end();
  }

  bool operator==(IteratorEnumerator rhs) {
    return current == rhs.current;
  }
};

template<typename IT, typename INDEXABLE_RANGE, typename SELECTOR>
struct SliceRange {
  IteratorFilter<IT, INDEXABLE_RANGE, SELECTOR> first;
  IteratorFilter<IT, INDEXABLE_RANGE, SELECTOR> last;

  decltype(first) begin() {
    return first;
  }

  decltype(first) end() {
    return last;
  }
};

template<typename INDEXABLE_RANGE, typename SELECTOR>
auto slice(INDEXABLE_RANGE &range,
           SELECTOR filter) -> SliceRange<decltype(std::declval<INDEXABLE_RANGE>().begin()), INDEXABLE_RANGE, SELECTOR> {
  SliceRange<decltype((static_cast<INDEXABLE_RANGE *>(nullptr)->begin())), INDEXABLE_RANGE, SELECTOR> it;

  it.first.container = &range; // destroying the container invalidates iterators
  it.first.selector = filter;
  it.last = it.first;

  it.first.current = range.begin();
  it.first.index = decltype(it.first.index)(0);
  it.last.current = range.end();

  // no es bueno.
  it.last.index = static_cast<decltype(it.first.index)>(std::distance(range.begin(), range.end()));

  return it;
}

#endif //STATISTICAL_ROBUST_SLICES_H
