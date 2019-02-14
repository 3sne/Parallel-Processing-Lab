__kernel void bto(__global int* bl, __global int* ol) {
    int i = get_global_id(0);
	int bin = bl[i];
	int ones = 0;
	int la;
	while (bin > 0) {
		la = bin % 10;
		ones = (ones * 10) + (la == 0);
		bin /= 10;
	}
	int n = ones;
	int reverse = 0;
	while (n != 0) {
		reverse = reverse * 10;
		reverse = reverse + n % 10;
		n = n/10;
   }
    ol[i] = reverse;
}