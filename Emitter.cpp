//
// Created by howorang on 01.02.19.
//

#include "Emitter.h"

Emitter emitter = Emitter();

void Emitter::simpleEmit(std::string text) {
    out += text;
}

void Emitter::saveToFile(std::string fileName) {
    std::ofstream outfile;
    outfile.open(fileName);
    outfile << out;
    outfile.close();
}
