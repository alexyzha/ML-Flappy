#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <iostream>

#ifndef GENE_POOL
#define GENE_POOL

    /*
     *  All possible genes
     */
    const std::string GENES = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890, .-;:_!\"#%&/()=?@${[]}";
    
    /*
     *  Number of possible genes
     */
    const int GENE_POOL_SIZE = 87;

    /*
     *  Population size
     */
    const int POPULATION_SIZE = 100;

    inline char SNP() {
        return GENES[rand() % GENE_POOL_SIZE];
    }

#endif

class Indiv {
public:
    Indiv(std::string s, const std::string& target);

    /*
     *  Individual's chromosome
     */
    std::string chromosome;

    /*
     *  Individual's fitness
     */
    int fitness;

    /*
     *  Length of target/chromosome
     */
    int len;

    /*
     *  Combines 2 strings with recombination
     */
    std::string mate(Indiv* other);

};

class GStr {
public:
    GStr(std::string t, int seed = time(0));

    void simulate(bool print = 1);

private:
    /*
     *  Chromosome size
     */
    int len;

    /* 
     *  Goal
     */
    std::string target;

    /*
     *  Generates random genome
     */
    std::string make_chromosome();

    /*
     *  Population
     */
    std::vector<Indiv*> current_population;

};