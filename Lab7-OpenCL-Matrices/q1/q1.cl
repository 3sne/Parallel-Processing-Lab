__kernel void weirdFlex(__global int *matA) {
    
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
    int currElement = matA[currElementIndex];
    int temp = 1;
    
    for ( int i = 1; i <= id_row + 1; i++ ) {
        temp *= currElement;
    }

    matA[currElementIndex] = temp;
}