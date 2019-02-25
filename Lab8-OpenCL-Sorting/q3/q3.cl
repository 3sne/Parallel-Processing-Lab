__kernel void wordCounter(__global char *s,
                          __global int *startInd,
                          __global int *endInd,
                          __global char *word,
                          __global int *wordCount) {

    int id = get_global_id(0);
    int start = startInd[id];
    int end = endInd[id];
    int flag = 1, k = 0;

    for ( int i = start; i <= end; i += 1 ) {
        if ( s[i] != word[k] ) {
            flag = 0;
            break;
        }
        k++;
    }

    if ( flag ) {
        wordCount[id] = 1;
    } else {
        wordCount[id] = 0;
    }
}