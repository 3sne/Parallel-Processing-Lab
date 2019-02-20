__kernel void dto(__global int* decList, __global int* octList) {
    int x = get_global_id(0);
    int decimalNumber = decList[x];
    int octalNumber = 0, i = 1;
    while (decimalNumber != 0) {
        octalNumber += (decimalNumber % 8) * i;
        decimalNumber /= 8;
        i *= 10;
    }
    octList[x] = octalNumber;
}