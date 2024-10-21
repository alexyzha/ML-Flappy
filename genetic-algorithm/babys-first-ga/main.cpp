#include "genetic.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cerr << "missing input" << std::endl;
        return -1;
    }
    GStr* gen = new GStr(argv[1]);
    gen->simulate();
    return 0;
}