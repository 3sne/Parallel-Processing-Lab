__kernel void selSort(__global char *str, __global char *out) {
	int id = get_global_id(0);
	int LS = get_global_size(0);
	int pos = 0;
	
	for ( int i = 0; i < LS; i++ ) {
		if ( ((int)str[id] > (int)str[i]) || (str[id] == str[i] && id < i)) {
			pos += 1;
		}
	}

	out[pos] = str[id];

}