//
// Created by martin on 2021-05-27.
//

#include "TIMatrix.h"

#include <cstdio>

/**
 * Favorite syntax for common scenarios
 * @return
 */

// index needs to evaluated via operator()?
declare_free_index(height)
declare_free_index(width)
declare_two_dim_mat(test_matrix2D, int, height, width)



void sample1(){
    auto i = 6_height;
    auto j = 4_width;
    test_matrix2D q(i,j);

    auto evens = [](const int idx) {
        return idx%2 == 0;
    };
    auto k = 1_height;

    for (auto& even_indexed_num: q[k]) {
        even_indexed_num = 4;
        printf("%d\n", even_indexed_num);
    }

}


int main(){
    sample1();
}