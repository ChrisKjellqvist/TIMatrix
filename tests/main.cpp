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

    for (auto even_indexed_num: q[i]) {
        even_indexed_num = 4;
        printf("%d\n", even_indexed_num);
    }

}

declare_one_dim_mat(oned, int, idx)

void sample2(){
    oned mx(11_idx);

    auto i = 0;
    for(auto& e: mx){
        e = i++;
    }

    auto evens = [](idx idx_val) {
        return idx_val.getN() % 2 == 0;
    };
    for(auto e: slice(mx, evens)){
        std::cout << e << "\n";
    }
}

int main(){
    sample2();
}