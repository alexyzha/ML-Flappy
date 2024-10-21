#include "game.h"

Game::Game(V<Bird*>& b, float spd, int seed) {
    srand(seed);
    //  Set training param
    passed = 0;
    cur_fitness = 0.0f;
    speed = spd;
    alive = b.size();
    //  Init bird pos/vel
    for(auto&i:b) {
        birds.push_back({i,sf::RectangleShape(sf::Vector2f(BIRD_RADIUS,BIRD_RADIUS))});
        birds.back().second.setPosition(175.0f,300.0f-(BIRD_RADIUS/2));
        bird_vel.push_back(0.0f);
    }
    //  Start with 6 pillars, 4-5 offscreen
    for(int i = 0; i < 6; ++i)
        generate_pillar();
}

void Game::run_game() {
    sf::Clock clock;
    while(alive && passed < 1000) {
        float d_time = clock.restart().asSeconds()*speed;
        int next_pillar = get_next_pillar();
        get_inputs(next_pillar);
        move_birds(d_time);
        move_pillars(d_time);
        kill_birds(next_pillar);
        cur_fitness += d_time;
    }
    if(alive) {
        for(auto&b:birds)
            if(b.first->fitness < 0.0f)
                b.first->fitness = cur_fitness,
                --alive;
    }
}

void Game::run_vis() {
    sf::RenderWindow window(sf::VideoMode(800,600),"SIM");
    sf::Clock clock;
    //  Load font
    sf::Font font;
    if(!font.loadFromFile("font/Hack-Regular.ttf")) {
        std::cerr << "INVALID FONT" << std::endl;
        return;
    }
    //  Make text
    sf::Text t_pillars,
             t_fitness;
    t_pillars.setFont(font);
    t_pillars.setCharacterSize(24);
    t_pillars.setFillColor(sf::Color::Green);
    t_pillars.setPosition(10.0f,10.0f);
    t_fitness.setFont(font);
    t_fitness.setCharacterSize(24);
    t_fitness.setFillColor(sf::Color::Green);
    t_fitness.setPosition(10.0f,40.0f);
    while(window.isOpen() && alive && passed < 210) {
        //  Window close event
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type == sf::Event::Closed)
                window.close();
        //  Delta time + all game related operations
        float d_time = clock.restart().asSeconds()*speed;
        int next_pillar = get_next_pillar();
        get_inputs(next_pillar);
        move_birds(d_time);
        move_pillars(d_time);
        kill_birds(d_time);
        cur_fitness += d_time;
        //  Update text
        t_pillars.setString("P: " + std::to_string(passed));
        t_fitness.setString("F: " + std::to_string(static_cast<int>(cur_fitness)));
        //  Render
        window.clear();
        for(const auto&b:birds)
            if(b.first->fitness < 0.0f)
                window.draw(b.second);
        for(const auto&p:pillars)
            window.draw(p.first),
            window.draw(p.second);
        //  Render text
        window.draw(t_pillars);
        window.draw(t_fitness);
        window.display();
    }
    if(alive) {
        for(auto&b:birds)
            if(b.first->fitness < 0.0f)
                b.first->fitness = cur_fitness,
                --alive;
    }
    if(window.isOpen())
        window.close();
}

void Game::diagnostics() {
    std::cout << (passed > 200 ? "\e[0;32m" : "\e[0;31m") << "Passed: " << passed << "\e[0m" << std::endl;
    std::cout << "Max fitness: " << cur_fitness << std::endl;
}

int Game::pillars_passed() {
    return passed;
}

void Game::generate_pillar() {
    //  y-range for opening [180,340], 160 range + 180 offset
    float x = (rand()%26)+25+(pillars.empty() ? 500.0f : (pillars.back().first.getPosition().x+BASE_DIST));
    float y = 300.0f-(PILLAR_OPENING/2)+(pillars.empty() ? (rand()%91)-45 : (rand()%181)-90);
    pillars.push_back({
        //  Top
        sf::RectangleShape(sf::Vector2f(PILLAR_WIDTH,y)),
        //  Bottom
        sf::RectangleShape(sf::Vector2f(PILLAR_WIDTH,600.0f-PILLAR_OPENING-y))
    });
    pillars.back().first.setPosition(x,0.0f);
    pillars.back().second.setPosition(x,y+PILLAR_OPENING);
}

void Game::move_pillars(float d_time) {
    int pop = 0;
    for(auto&p:pillars) {
        p.first.move(-SCROLL_SPEED*d_time,0.0f);
        p.second.move(-SCROLL_SPEED*d_time,0.0f);    
        if(p.first.getPosition().x+PILLAR_WIDTH < 0.0f)
            ++pop;
    }
    while(pop--) {
        pillars.pop_front();
        ++passed;
        generate_pillar();
    }
}

void Game::move_birds(float d_time) {
    for(int i = 0; i < birds.size(); ++i) {
        if(birds[i].first->fitness > 0.0f)
            continue;
        birds[i].second.move(0.0f,bird_vel[i]*d_time);
        bird_vel[i] += GRAVITY*d_time;
    }
}

void Game::kill_birds(int next_pillar) {
    for(auto&b:birds) {
        if(b.second.getPosition().y < 0.0f ||
           b.second.getPosition().y > (600.0f-BIRD_RADIUS) || 
           b.second.getGlobalBounds().intersects(pillars[next_pillar].first.getGlobalBounds()) ||
           b.second.getGlobalBounds().intersects(pillars[next_pillar].second.getGlobalBounds())) {
            if(b.first->fitness < 0.0f) {
                b.first->fitness = cur_fitness;
                --alive;
            }
        }
    }
}

void Game::get_inputs(int next_pillar) {
    for(int i = 0; i < birds.size(); ++i) {
        if(birds[i].first->fitness != -1)
            continue;
        //  Generate inputs for network
        V<float> inputs;
        inputs.push_back(pillars[next_pillar].first.getPosition().x);  //  Pillar x
        inputs.push_back(pillars[next_pillar].first.getSize().y);      //  Top pillar y
        inputs.push_back(inputs.back()+PILLAR_OPENING);                //  Bottom pillar y
        inputs.push_back(birds[i].second.getPosition().y);             //  Bird y
        inputs.push_back(bird_vel[i]);                                 //  Bird vel
        //  Jump if given signal to jump
        if(birds[i].first->f_prop(inputs))
            bird_vel[i] = JUMP_STR;
    }
}

int Game::get_next_pillar() {
    int ret = 0;
    while(pillars[ret].first.getPosition().x+PILLAR_WIDTH < BIRD_X)
        ++ret;
    return ret;
}