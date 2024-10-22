#include <algorithm>
#include "evo.h"

Evolver::Evolver(float stdev, float spd, int seed_) : rng(std::random_device{}()), dist(0.0f,stdev) {
    srand(seed_);
    seed = seed_;
    speed = spd;
    for(int i = 0; i < POP_SIZE; ++i) {
        V<float> genes = chromosome(18);
        c_population.push_back(new Bird(genes));
    }
}

void Evolver::simulate(int generations, std::string f_path) {
    for(int i = 1; i <= generations; ++i) {
        std::cout << "#=====================#" << std::endl;
        std::cout << "Running generation: " << i << std::endl;
        Game* cur = new Game(c_population,speed,seed+i);
        //  Watch every 10
        //  !(i%10) ? cur->run_vis() : cur->run_game();
        //  Watch every:
        cur->run_vis();
        //  Print diagnostics
        cur->diagnostics();
        int passed = cur->pillars_passed();
        delete cur;
        //  Population control
        sort(begin(c_population),end(c_population),[](Bird* a, Bird* b){
            return a->fitness > b->fitness;
        });
        //  Save if good genes
        if(passed > 200)
            save(f_path,0);
        //  Retain information from last generation
        if(i == generations)
            return;
        //  Seed new population
        V<Bird*> n_population;
        //  Top 10% live (10% new population)
        for(int i = 0; i < POP_SIZE/10; ++i)
            n_population.push_back(c_population[i]);
        //  Selectively breed top 20% with 20% mutation rate (40% new population)
        for(int i = 0; i < POP_SIZE/10*4; ++i) {
            int a = rand() % (POP_SIZE/10*2);
            int b = rand() % (POP_SIZE/10*2);
            while(b == a)
                b = rand() % (POP_SIZE/10*2);
            V<float> n_gene = mate(c_population[a],c_population[b],20);
            n_population.push_back(new Bird(n_gene));
        }
        //  Selectively breed top 40% with 40% mutation rate (50% new population)
        for(int i = 0; i < POP_SIZE/10*5; ++i) {
            int a = rand() % (POP_SIZE/10*4);
            int b = rand() % (POP_SIZE/10*4);
            while(b == a)
                b = rand() % (POP_SIZE/10*4);
            V<float> n_gene = mate(c_population[a],c_population[b],40);
            n_population.push_back(new Bird(n_gene));
        }
        /*
        for(int i = POP_SIZE/10; i < POP_SIZE; ++i)
            delete c_population[i];
        */
        swap(c_population,n_population);
        for(auto&i:c_population)
            i->fitness = -1.0f;
    }
    std::cout << "#=====================#" << std::endl;
}

void Evolver::save(std::string f_path, int index) {
    std::ofstream file(f_path);
    if(!file)
        return;
    //  Save fitness
    file << c_population[index]->fitness << std::endl;
    //  Save input -> hidden weights
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 5; ++j)
            file << c_population[index]->i_weights[i][j] << std::endl;
    //  Save hidden -> output weights
    for(int i = 0; i < 3; ++i)
        file << c_population[index]->h_weights[i] << std::endl;
    file << std::endl;
    file.close();
}

void Evolver::save_all(std::string f_path, float thresh) {
    std::ofstream file(f_path);
    if(!file)
        return;
    auto sv = [&](int index) {
        //  Save fitness
        file << c_population[index]->fitness << std::endl;
        //  Save input -> hidden weights
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 5; ++j)
                file << c_population[index]->i_weights[i][j] << std::endl;
        //  Save hidden -> output weights
        for(int i = 0; i < 3; ++i)
            file << c_population[index]->h_weights[i] << std::endl;
        file << std::endl;
    };
    //  Save all
    for(int i = 0; i < c_population.size(); ++i)
        if(c_population[i]->fitness > thresh)
            sv(i);
    //  Close file
    file.close();
}

float Evolver::snp() {
    return dist(rng);
}

V<float> Evolver::mate(Bird* a, Bird* b, int m_rate) {
    V<float> ret;
    //  Thresholds
    int t1 = (100-m_rate)/2,
        t2 = (100-m_rate);
    //  Mutate input -> hidden weights
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 5; ++j) {
            int r = rand() % 100;
            if(r < t1) ret.push_back(a->i_weights[i][j]);
            else if(r < t2) ret.push_back(b->i_weights[i][j]);
            else ret.push_back(snp());
        }
    }
    //  Mutate hidden -> output weights
    for(int i = 0; i < 3; ++i) {
        int r = rand() % 100;
        if(r < t1) ret.push_back(a->h_weights[i]);
        else if(r < t2) ret.push_back(b->h_weights[i]);
        else ret.push_back(snp());
    }
    return ret;
}

V<float> Evolver::chromosome(int len) {
    V<float> ret;
    for(int i = 0; i < len; ++i)
        ret.push_back(snp());
    return ret;
}