#include "genetic.h"

Indiv::Indiv(std::string s, const std::string& target) {
    fitness = 0;
    chromosome = s;
    len = chromosome.size();
    for(int i = 0; i < len; ++i)
        fitness += !!(target[i]^s[i]);
}

std::string Indiv::mate(Indiv* other) {
    std::string ret = "";
    for(int i = 0; i < len; ++i) {
        int p_mut = rand() % 100;
        if(p_mut < 45)
            ret.push_back(this->chromosome[i]);
        else if(p_mut < 90)
            ret.push_back(other->chromosome[i]);
        else
            ret.push_back(SNP());
    }
    return ret;
}

/*=====================================================================*/
// keep 10% of fittest population
// fill in the rest with offspring with the top 50% of fittest

GStr::GStr(std::string t, int seed) {
    srand(seed);
    target = t;
    len = t.size();
    current_population = std::vector<Indiv*>(POPULATION_SIZE);
    for(auto&s:current_population)
        s = new Indiv(make_chromosome(),target);
}

std::string GStr::make_chromosome() {
    std::string ret = "";
    for(int i = 0; i < len; ++i)
        ret.push_back(SNP());
    return ret;
}

void GStr::simulate(bool print) {
    bool found = 0;
    int generation = 1;
    while(!found) {
        std::sort(begin(current_population),end(current_population),[](Indiv* a, Indiv* b) {return a->fitness < b->fitness;});
        //  Print generation
        if(print) {
            std::cout << "#========================================#" << std::endl;
            std::cout << "Generation: " << generation << std::endl;
            std::cout << "Fittest: " << current_population[0]->chromosome << std::endl;
            std::cout << "Fitness: " << current_population[0]->fitness << std::endl;
            std::cout << "Most unfit: " << current_population.back()->chromosome << std::endl;
            std::cout << "Highest fitness: " << current_population.back()->fitness << std::endl;
        }
        if(!current_population[0]->fitness)
            found = 1;
        std::vector<Indiv*> next_population;
        //  Elitism, keep top 10%
        for(int i = 0; i < POPULATION_SIZE*10/100; ++i)
            next_population.push_back(current_population[i]);
        //  Mate top 50%
        for(int i = 0; i < POPULATION_SIZE*90/100; ++i) {
            int top_50 = POPULATION_SIZE*50/100;
            int r1 = rand() % top_50;
            int r2 = rand() % top_50;
            while(r2 == r1)
                r2 = rand() % top_50;
            std::string temp = current_population[r1]->mate(current_population[r2]);
            next_population.push_back(new Indiv(temp,target));
        }
        std::swap(next_population,current_population);
        ++generation;
    }
    //  Pretty print
    if(print)
        std::cout << "#========================================#" << std::endl;
}