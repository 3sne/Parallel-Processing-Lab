/*
 * q4FileWordFrequency.c
 *
 *  Created on: 08-Jan-2019
 *      Author: student
 */

#include <mpi.h>
#include <stdio.h>
#include <string.h>

typedef struct entry { 
    char word[256];
    int frequency;
}entry;

struct entry* wordList[256];
int l = 0;
int buf[256];

struct entry* newEntry(char s[]) {
    struct entry* this;
    this = (struct entry*)malloc(sizeof(struct entry));
    strcpy(this->word, s);
    return this;
}

int searchForWord() {
    for ( int i = 0; i < l; i++ ) {
        if( strcmp(buf, wordList[i]->word) == 0 ) {
            return i;
        }
    }
    return 0;
} 

void sortAndPrint() {
    struct entry* temp;
    for ( int i = 0; i < l; i++ ) {
        for ( int j = 0; j < l - i - 1; j++ ) {
            if ( wordList[j]->frequency < wordList[j + 1]->frequency ) {
                temp = wordList[j];
                wordList[j] = wordList[j + 1];
                wordList[j + 1] = temp;
            }
        }
    }

    for ( int i = 0; i < l; i++ ) {
        printf("Word: %s | Freq: %d", wordList[i]->word, wordList[i]->frequency);
    }
}

int main(int argc, char const *argv[])
{
    struct entry* temp;
    int cap;
    FILE *f;
    f = fopen("SampleText.txt", "r");
    
    while( buf != EOF ) {
        fscanf(f, "%s", buf);
        if ( (cap = searchForWord()) == 0) {
            temp = newEntry(buf);
            wordList[l++] = temp;
        } else {
            wordList[cap]->frequency += 1;
        }
    }

    sortAndPrint();

    return 0;
}
