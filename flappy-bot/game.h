#include <time.h>
#include <queue>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "bird.h"

#ifndef PAIR_MACRO
#define PAIR_MACRO
using pf = std::pair<float,float>;
#endif

class Game {
public:
    /*
     *  @param b vector of bird*
     *  @param disp open display
     *  @param seed default = sys time
     */
    Game(V<Bird*>& b, float spd, int seed = time(0));

    /*
     *  Play until all birds are dead or 1000 pillars passed
     */
    void run_game();

    /*
     *  Play with window
     */
    void run_vis();

    /*
     *  Print max passed/fitness
     */
    void diagnostics();

    /*
     *  Return number pased
     */
    int pillars_passed();

private:
    int alive;
    int passed;
    float speed;
    float cur_fitness;
    const float BIRD_RADIUS = 36.0f;
    const float SCROLL_SPEED = 200.0f;
    const float PILLAR_WIDTH = 75.0;
    const float PILLAR_OPENING = 250.0f;
    const float BASE_DIST = 300.0f;
    const float BIRD_X = 170.0f;
    const float GRAVITY = 1000.0;
    const float JUMP_STR = -400.0f;
    V<std::pair<Bird*,sf::RectangleShape>> birds;
    V<float> bird_vel;
    std::deque<std::pair<sf::RectangleShape,sf::RectangleShape>> pillars;

    /*
     *  Creates a pillar based on the latest pillar
     */
    void generate_pillar();

    /*
     *  Moves all pillars
     */
    void move_pillars(float d_time);

    /*
     *  Moves all birds
     */
    void move_birds(float d_time);

    /*
     *  - Kills all birds that are colliding
     *  - Assigns fitness
     */
    void kill_birds(int next_pillar);

    /*
     *  Asks bird to jump
     */
    void get_inputs(int next_pillar);

    /*
     *  Gets next target pillar
     */
    int get_next_pillar();

};