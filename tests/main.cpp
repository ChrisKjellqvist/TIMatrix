//
// Created by martin on 2021-05-27.
//

#include "TIMatrix.h"
#include "Slices.h"

#include <cstdio>
#include <iostream>

/**
 * Favorite syntax for common scenarios
 * @return
 */

// index needs to evaluated via operator()?

declare_two_dim_mat(test_matrix2D, int, height, width)

void sample1(){
    auto i = 6_height;
    auto j = 4_width;
    test_matrix2D q(i,j);

    i = 1_height;
//    TODO - we want this to work
//    i = height(j);

    for (auto even_indexed_num: q[i]) {
        even_indexed_num = 4;
        printf("%d\n", even_indexed_num);
    }

}

declare_one_dim_mat(oned, int, idx)

#include <algorithm>

void sample2(){
    oned mx(11_idx);

    auto i = 0;
    auto monotonic = [&i](){
        return i++;
    };
    std::generate(mx.begin(), mx.end(), monotonic);

    auto evens = [](idx idx_val) {
        return idx_val.getN() % 2 == 0;
    };
    for(auto e: slice(mx, evens)){
        std::cout << e << "\n";
    }
}

void sample3(){
    oned mx(11_idx);

    auto i = 2;
    auto monotonic = [&i](){
        i *= 2;
        return i;
    };

    auto evens = [](idx idx_val) {
        return idx_val.getN() % 2 == 0;
    };
    auto evenslice = slice(mx, evens);
    std::generate(evenslice.begin(), evenslice.end(), monotonic);

    for(auto e: slice(mx, evens)){
        std::cout << e << "\n";
    }
}
int main(){
    sample3();

}