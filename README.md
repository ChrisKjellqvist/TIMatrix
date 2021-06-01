# Typed Index Matrix

Templated multi-dimensional array class that supports having a unique type for each
dimension in the matrix. A two-dimensional array might have an x and y dimension,
and now we can have x dimension and y dimension types. Incorrect uses of these types
results in compile time errors. Using `int`s as an index types now requires explicitly
allowing that behavior.

## Examples

### Basic Example
```c++
// amount you need to pay your employee
double *paycheck_amts;
// amount of sales tax collected during a transaction
double *sales_tax;

void pay_employees() {
  for (int i = 0; i < num_employees(); ++i) {
    // accidentally used wrong array!! Now employees are being paid a fraction
    // of what they're owed!
    pay_employee_by_id(i, sales_tax[i]);
  }
}
```

In the above example, there are two arrays with the same type signatures but
are indexed by distinctly different value types (employee ID and receipt number).
But because types match when using `int`s to index, the compiler will let this
erroneous behavior fly... 

Consider how typed indexes can help with the following solution.
``` c++
// introduce index types
declare_free_index(employee_id);
declare_free_index(receipt_num);
// declare matrices that use these index types
// paychecks are `double` typed and are indexed by employee_id
declare_one_dim_mat(paycheck_ar, double, employee_id);
// sales taxes are also `double` values but are indexed by receipt number
declare_one_dim_mat(sales_tax_ar, double, receipt_num);

paycheck_ar paycheck_amts;
sales_tax_ar sales_tax;

void pay_employees() {
  for (employee_id i(0); i < num_employees(); ++i) {
  // ERROR! No conversion from employee_id to receipt_num!
    pay_employee_by_id(i, sales_tax[i]);
  }
}
```

In this scenario, the for loop uses an employee_id instead of an int,
resulting in a compile time error that prevents indexing into sales tax
with an employee ID.

### Multi-Dimensional Array Example

```c++
void clear_game_board(int** board, int width, int height) {
  for(int i = 0; i < width; ++i) {
    for(int j = 0; j < height; ++j) {
      board[i][j] = 0;
    }
  }
}
```

Is this code correct? Hard to know without checking out the initialization of the array
to see if the developer is doing height or width first for dimensions into the board.
The code compiles, so the only way of knowing is when you get a segfault...

```c++
void clear_game_board(TwoDimMat<int, y_dim, x_dim> *board, x_dim width, y_dim height) {
  for (x_dim i(0); i < width; ++i) {
    for (y_dim j(0); j < height; ++j) {
      // ERROR! in indexing
      board[i][j] = 0;
      // board[j][i] = 0 works!
    }
  }
}
```

Since each dimension of a multi-dimensional array can have a different type, misindexing
into dimensions is now a thing of the past. But what if you **want** to compute the
transpose? Well... then you have several options, but you have to do both _explicitly_.
Here's one such approach that shows explicit conversion between types.

```c++
// compute transpose
// i,j now have the correct types to index in as [i][j] 
for (y_dim i(0); i < height; ++i) {
  for (x_dim j(0); j < width; ++j) {
    auto &start = my_matrix[i][j];
    auto &transposition = my_matrix[index_cast<y_dim>(j)][index_cast<x_dim>(i)];
    swap(start, transposition);
  }
}
```

This template function `index_cast<>` is just shorthand for the strikingly ugly manual
cast `x_dim(int(i))`. 