__kernel void toggle(__global char *A, __global char *B) {
	int id = get_global_id(0);
	int rev = 0;
	int n = (int)A[id];
	while ( n > 0 ) {
		rev = rev * 10 + (n % 10);
		n /= 10;
	}
	B[id] = (char)rev;
}