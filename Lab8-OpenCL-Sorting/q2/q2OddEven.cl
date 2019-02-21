__kernel void OddEven(__global int* list) {
    int id = get_global_id(0);
    int a = list[id];
    int t = list[id + 1];

    if ( id % 2 == 1 ) {
        if ( a > t ) {
            list[id] = t;
            list[id + 1] = a;
        }
    }
}