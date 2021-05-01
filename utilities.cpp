#include "utilities.h"
//finds the toReplace in s and replaces the substring with replacement
void findAndReplace(string &s, string &toReplace, string &replacement){

    size_t index = 0;
    /* Locate the substring to replace. */
    index = s.find(toReplace, index);

    while (index!= -1){
        /* Make the replacement. */
        s.replace(index, toReplace.length(), replacement);
        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index = s.find(toReplace, index);
    }
}