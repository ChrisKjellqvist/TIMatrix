# Typed Index Matrix

Templated multi-dimensional array class that supports having a unique type for each
dimension in the matrix. A two-dimensional array might have an x and y dimension,
and now we can have x dimension and y dimension types. Incorrect uses of these types
results in compile time errors. Of course you can still use ints to index into the
matrix because preventing that might be unnecessarily prohibitive. Something to think
about...

###Reasoning
I've lately had to write a lot of code with high dimension matrixes and I
often run into problems where I accidentally use `i` or `j` in the wrong position
when indexing into these matrices. Sometimes I use `i` twice, etc... It's not
totally uncommon. What occurred to me though is that each dimension has a semantic
meaning and the type `int` does not. You could rename all of your index variable
names to `row` and `column` but even so you'd have to remember which dimension
corresponds to `row` and such. Better naming is a decent solution but not as
powerful as compile time checking!

Consider the following example. You're working on a codebase that you're unfamiliar
with and you see the input is this array.
```c++
int xfc_512b_f(int *****ar) {
  ...
  for (int i = 0; i < func1(); ++i) {
    for (int j = 0; j < func2(); ++j) {
      for (int k = 0; k < func3(); ++k) {
        // some later use of the array
        int q = ar[i][j][i][4][k];        
      }
    }
  }
}
```

This is not always fun to try to figure out. You'd have to look at call sites for
`xfc_512b_f` and try and deduce what the input arguments are and what each dimension
of the array means. And even if you know what the dimensions mean, did the developer
_mean_ to use the index `i` twice or is that the bug? Hard to say.

With Typed Index Matrices it is much harder to purposefully misuse indexes, and the
definition of each dimension now becomes explicit like so.

```c++
// Some header file

// Declare index types
indexify(day_idx);
indexify(employee_id_idx);
indexify(report_idx);
declare_three_dim_mat(my_matrix, int, day_idx, employee_id_idx, report_idx);

// Some source file
int file_report(my_matrix &ar) {
  ... 
  for (day_idx i = 0; i < 365; ++i) {
    for (employee_id_idx j = 0; j < n_emps(); ++j) {
      for (report_idx k = 0; k < f(j); ++k) {
        ...
        // this results in a compile time error because index j (the employee's id) is used in the
        // dimension for report id
        do_thing(ar[i][j][j]);
        // This would compile correctly
        do_thing(ar[i][j][k]);
      }
    }
  }
}
```
Even though the above code has terrible naming for its variables, typos can still be found by the type checker.
AND even if the programmer refuses to use the explicit types given for the dimension and insists on using `int`, 
the poor soul forced to debug his code can at least find the declaration of the `my_matrix` type and find the 
meaning of each dimension.