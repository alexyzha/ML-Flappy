#include <string>
#include <fstream>
#include "game.h"

void run(std::string f_path, int max_birds, float speed = 1.0f, int seed = time(0)) {
    std::ifstream file(f_path);
    if(!file) {
        std::cerr << "invalid file path" << std::endl;
        return;
    }
    V<Bird*> birds;
    //  Get all inputs
    for(int i = 0; i < max_birds; ++i) {
        V<float> chromosome(18);
        for(int j = 0; j < 18; ++j) if(!(file >> chromosome[j])) {
            std::cerr << "not enough chromosome, " << i << ':' << j << std::endl;
            return;
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        birds.push_back(new Bird(chromosome));
    }
    //  Run game instance
    Game* cur_game = new Game(birds,speed,seed);
    cur_game->play();
    delete cur_game;
    for(auto&bird:birds)
        delete bird;
}