#include "utilities.h"

void findAndReplace(string &s, string &toReplace, string &replacement){

    size_t index = 0;
    index = s.find(toReplace, index);
    while (index!= -1){

        /* Locate the substring to replace. */
        if (index == std::string::npos)
            break;

        /* Make the replacement. */
        s.replace(index, toReplace.length(), replacement);

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index = s.find(toReplace, index);

    }

}
