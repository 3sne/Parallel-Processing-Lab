kernel void wordCounter(__global char *s,
                        __global int *startInd,
                        __global int *endInd,
                        __global char *word,
                        __global int *wordCount) {

    int id = get_global_id(0);
    int start = startInd[id];
    int end = endInd[id];
    int flag = 1;
    for ( int i = start, int k = 0; i < end; i++, k++) {
        if ( s[i] != word[k] ) {
            flag = 0;
            break;
        }
    }

    if ( flag ) {
       (*wordCount)++;
    }
}