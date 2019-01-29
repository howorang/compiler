#include <iostream>
#include "global.h"

int main(int argc, char** argv) {
    ++argv, --argc;
    yyin = fopen("arithtest.pas", "r");
    yyparse ();
    exit (0);
}