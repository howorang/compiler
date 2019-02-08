#include <iostream>
#include "global.h"

int main(int argc, char** argv) {
    ++argv, --argc;
    yyin = fopen("bubblesort.pas", "r");
    yyparse ();
    exit (0);
}