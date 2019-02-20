__kernel void reverseIt(__global char *s, int len) {
    int i = get_global_id(0);
    char temp = s[i];
    s[i] = s[len - i - 1];
    s[len - i - 1] = temp;
}