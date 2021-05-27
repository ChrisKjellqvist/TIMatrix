//
// Created by martin on 2021-05-27.
//

#ifndef STATISTICAL_ROBUST_SLICES_H
#define STATISTICAL_ROBUST_SLICES_H

template<typename IT, typename INDEXABLE_RANGE, typename SELECTOR>
struct IteratorFilter{
    INDEXABLE_RANGE* container;
    SELECTOR selector;
    IT current;

    using size_type = typename INDEXABLE_RANGE::size_type;
    size_type index = size_type(0);

    // Do we do arithmetic as well? we probably should
    IteratorFilter operator++(){
        do{
            index++;
            current++;
        }while(!selector(index) && current != container->end());
        return *this;
    }
    IteratorFilter operator++(int){
        return ++(*this);
    }
    decltype(*(IT()))& operator *(){
        return *current;
    }
    // Hack you say? We can't know what out of range is or can we?
    bool operator!=(IteratorFilter rhs){
        //return index < rhs.index;
        return current != container->end();
    }
    bool operator==(IteratorFilter rhs){
        return current == rhs.current;
    }
};

template<typename IT, typename INDEXABLE_RANGE, typename SELECTOR>
struct SliceRange{
    IteratorFilter<IT, INDEXABLE_RANGE, SELECTOR> first;
    IteratorFilter<IT, INDEXABLE_RANGE, SELECTOR> last;
    decltype(first) begin(){
        return first;
    }
    decltype(first) end(){
        return last;
    }
};

template<typename INDEXABLE_RANGE, typename SELECTOR>
auto slice(INDEXABLE_RANGE& range, SELECTOR filter) -> SliceRange<decltype(std::declval<INDEXABLE_RANGE>().begin()), INDEXABLE_RANGE, SELECTOR>{
    SliceRange<decltype((static_cast<INDEXABLE_RANGE*>(nullptr)->begin())), INDEXABLE_RANGE, SELECTOR> it;

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
