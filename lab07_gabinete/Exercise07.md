# Exercise 07
Create two programs (<code>lab07_surname_A.c</code> and <code>lab07_surname_B.c</code>) that will communicate using a shared memory to compute for the [matrix product](https://en.wikipedia.org/wiki/Matrix_multiplication) of two matrices.

<code>lab07_surname_A.c</code> should read a file named **input.txt** (format below) for the matrices to be multiplied. <code>lab07_surname_A.c</code> will compute the left half of the answer while <code>lab07_surname_B.c</code> will compute the right half of the answer. If the number of columns for the answer is odd, <code>lab07_surname_A.c</code> will compute for the values in the middle column. Once every cell in the answer is computed, <code>lab07_surname_B.c</code> must print in the terminal the matrix product.

Note: Your program MUST check if matrix multiplication can be done on the given matrices. That is, if the number of columns of Matrix A is not equal to the number of rows of Matrix B, multiplication can’t be done on the two matrices, and your program must print an error message

FORMAT OF **input.txt**:

    rowA colA           // # of rows and cols of Matrix A
    e00 e01 e02 …       // elements of Matrix A
    e10 e11 e12 …
    …
    rowB colB           // # of rows and cols of Matrix B
    e00 e01 e02 …       // elements of Matrix B
    e10 e11 e12 …
    …

SAMPLE **input.txt** file: 

    5 5
    1 2 3 4 5
    2 3 4 5 6
    3 4 5 6 7
    4 5 6 7 8
    5 6 7 8 9
    5 3
    1 2 3
    2 3 4
    3 4 5
    4 5 6
    5 6 7

SAMPLE output on the terminal:

    55 70 85
    70 90 110
    85 110 135
    100 130 160
    115 150 185

TIPS
1. Create a shared memory for matA, matB, and matResult separately
2. Create another shared memory for matrix_sizes = [a_row, b_row, a_col, b_col]
3. Convert everything in 1D array (matA, matB, and matResult)
4. Create algorithm and computation for matrix multiplication (indexing, depends on size of array)
5. Use sleep (before clearing)
6. 4 shared memory locations: matA, matB, matResult (resulting matrix), and  matrix_sizes
7. 4 keys need to define 
