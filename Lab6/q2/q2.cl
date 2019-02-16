__kernel void fore(__global char *A, 
				   __global char *B,
				   int n,
				   int strlen) {

	int i = get_global_id(0);
	char myChar = A[i];
	for(int j = 0; j < n; j++) {
		B[strlen * j + i] = myChar;
	}
}