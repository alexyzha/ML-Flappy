#include <string>
#include <random>
#include <fstream>
#include "game.h"

class Evolver {
public:
    /*
     *  @param stdev standard deviation
     *  @param seed sys time or custom def
     */
    Evolver(float stdev, float spd, int seed_ = time(0));

    /*
     *  Runs simulation for x generations
     */
    void simulate(int generations, std::string f_path = "");

    /*
     *  Saves a bird from the current population into a file
     */
    void save(std::string f_path, int index);

    /*
     *  Saves all birds above threshold
     */
    void save_all(std::string f_path, float thresh = -2.0f);

private:
    int seed;
    float speed;
    const int POP_SIZE = 30;
    std::mt19937 rng;
    std::normal_distribution<float> dist;
    V<Bird*> c_population;

    /*
     *  Generates random mutation (mean = 0, stdev = stdev (0.1))
     */
    float snp();

    /*
     *  Mates 2 birds:
     *  - 45% a allele, 45% b allele, 10% mutation
     */
    V<float> mate(Bird* a, Bird* b, int m_rate = 10);

    /*
     *  Generates chromosome of length x
     */
    V<float> chromosome(int len);

};