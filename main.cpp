#include <iostream>
#include "global.h"

int main(int argc, char** argv) {
    ++argv, --argc;
    yyin = fopen(*argv, "r");
    yyparse ();
    exit (0);
}