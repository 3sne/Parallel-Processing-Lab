__kernel void reverseAll(__global char *s, __global int *startInd, __global int *endInd) {
    int id = get_global_id(0);
    int start = startInd[id];
    int end = endInd[id];
    printf("id > %d | start > %d  | end > %d\n", id, start, end);
    for ( int i = 0; i <= ((end - start) / 2) ; i++ ) {
        int temp = s[start + i];
        s[start + i] = s[end - i];
        s[end - i] = temp;
    }
}