__kernel void superSummer(__global int *matA, __global int *matB) {
    
    /* 
        NOTE:
            To easily understand element selection, think:
                Column in direction-0 or X
                Row in direction-1 or Y
                (Just like convention for a matrix in maths)
            This also goes along w.r.t 2D array implementation in C, which is:
                A[1][2] means row 1 and column 2 in 2D array A >>
                    ---[X|0]--->
                 A  0  1  2  3  4  5
            |    0  .  .  .  .  .  .
          [Y|1]  1  .  .  x  .  .  .
            |    2  .  .  .  .  .  .
            v    3  .  .  .  .  .  .

     */

    int id_col = get_global_id(0);
    int id_row = get_global_id(1);
    int col_size = get_global_size(0);
    int row_size = get_global_size(1);
    int currElementIndex = (id_row * col_size) + id_col;

    int colSum = 0, rowSum = 0;
    
    for ( int i = 0; i < col_size; i++ )        //column sum
        colSum += matA[id_row * col_size + i];
    
    for ( int i = 0; i < row_size; i++ )        //row sum
        rowSum += matA[i * col_size + id_col];

    matB[currElementIndex] = colSum + rowSum;
}