kernel void swep(__global int *arr) {
    int i = get_global_id(0);
    if ( i % 2 == 1) {
        int temp = arr[i];
        arr[i] = arr[i - 1];
        arr[i - 1] = temp;
    }
}