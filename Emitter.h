//
// Created by howorang on 01.02.19.
//

#ifndef COMPILER_EMITTER_H
#define COMPILER_EMITTER_H

#include <string>
#include "global.h"

class Emitter {
public:
    void simpleEmit(std::string text);
private:
    std::string out;

};
#endif //COMPILER_EMITTER_H
