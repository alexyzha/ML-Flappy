#include <random>
#include "evo.h"

int main(int argc, char* argv[]) {
    Evolver E(0.1f,3.0f,7);
    E.simulate(50,"gene_sample.txt");
    E.save_all("output.txt");
    return 0;
}