#include "game.h"

Game::Game(V<Bird*>& b, float spd, int seed) {
    srand(seed);
    //  Set param
    passed = 0;
    next_pillar = 0;
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

void Game::play() {
    //  Initialize
    sf::RenderWindow window(sf::VideoMode(800,600),"SIM");
    sf::Clock clock;
    //  Load font
    sf::Font font;
    if(!font.loadFromFile("font/Hack-Regular.ttf")) {
        std::cerr << "FONT NOT FOUND" << std::endl;
        return;
    }
    //  Make text, set param
    sf::Text t_pillars, t_alive;
    set_text(t_pillars,font,24,sf::Color::Green,10.0f,10.0f);
    set_text(t_alive,font,24,sf::Color::Green,10.0f,40.0f);
    //  Main game loop, inputs from bird(s) neural networks
    while(window.isOpen() && alive) {
        //  Listen for manual window close
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type == sf::Event::Closed)
                window.close();
        //  Get delta time * speed
        float d_time = clock.restart().asSeconds()*speed;
        get_next_pillar();
        //  Main operations
        get_inputs();
        move_birds(d_time);
        move_pillars(d_time);
        kill_birds();
        //  Update text
        t_pillars.setString("P: " + std::to_string(passed));
        t_alive.setString("A: " + std::to_string(alive));
        //  Render
        window.clear();
        for(const auto&b:birds) if(b.first->alive) {
            window.draw(b.second);
            draw_vision(window,sf::Color::Red,sf::Color::Blue,sf::Color::Green);
        }
        for(const auto&p:pillars)
            window.draw(p.first),
            window.draw(p.second);
        //  Render text
        window.draw(t_pillars);
        window.draw(t_alive);
        window.display();
    }
    if(window.isOpen())
        window.close();
    std::cout << "#=================#" << std::endl;
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "#=================#" << std::endl;
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
        --next_pillar;
        generate_pillar();
    }
}

void Game::move_birds(float d_time) {
    for(int i = 0; i < birds.size(); ++i) {
        if(!birds[i].first->alive)
            continue;
        birds[i].second.move(0.0f,bird_vel[i]*d_time);
        bird_vel[i] += GRAVITY*d_time;
    }
}

void Game::kill_birds() {
    for(auto&b:birds) {
        if(b.second.getPosition().y < 0.0f ||
           b.second.getPosition().y > (600.0f-BIRD_RADIUS) || 
           b.second.getGlobalBounds().intersects(pillars[next_pillar].first.getGlobalBounds()) ||
           b.second.getGlobalBounds().intersects(pillars[next_pillar].second.getGlobalBounds())) {
            if(b.first->alive) {
                b.first->alive = 0;
                --alive;
            }
        }
    }
}

void Game::get_inputs() {
    //  For all birds currently alive
    for(int i = 0; i < birds.size(); ++i) {
        if(!birds[i].first->alive)
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

void Game::get_next_pillar() {
    if(pillars[next_pillar].first.getPosition().x+PILLAR_WIDTH < BIRD_X)
        ++next_pillar,
        ++passed;
}

void Game::draw_vision(sf::RenderWindow& window, sf::Color col1, sf::Color col2, sf::Color col3) {
    for(int i = 0; i < birds.size(); ++i) {
        if(!birds[i].first->alive)
            continue;
        //  Bird pos
        sf::Vector2f pos = birds[i].second.getPosition();
        float mid_y = pos.y+(BIRD_RADIUS/2);
        float mid_x = pos.x + (BIRD_RADIUS / 2);
        //  Position of next pillar pair
        sf::Vector2f top_pos = pillars[next_pillar].first.getPosition();
        sf::Vector2f bot_pos = pillars[next_pillar].second.getPosition();
        float top_y = top_pos.y+pillars[next_pillar].first.getSize().y;
        float bot_y = bot_pos.y;
        //  Velocity line
        float vel = bird_vel[i];
        float vel_len = std::abs(vel)*0.2f;
        float vel_end = mid_y-(vel_len*(vel < 0 ? 1 : -1));
        //  Draw line to top
        sf::Vertex top_line[] = {
            sf::Vertex(sf::Vector2f(pos.x+(BIRD_RADIUS/2),mid_y),col1),
            sf::Vertex(sf::Vector2f(top_pos.x,top_y),col1)
        };
        //  Draw line to bottom
        sf::Vertex bot_line[] = {
            sf::Vertex(sf::Vector2f(pos.x+(BIRD_RADIUS/2),mid_y),col2),
            sf::Vertex(sf::Vector2f(bot_pos.x,bot_y),col2)
        };
        //  Draw velocity line
        sf::Vertex vel_line[] = {
            sf::Vertex(sf::Vector2f(mid_x,mid_y),col3),
            sf::Vertex(sf::Vector2f(mid_x,vel_end),col3)
        };
        //  Draw lines
        window.draw(top_line,2,sf::Lines);
        window.draw(bot_line,2,sf::Lines);
        window.draw(vel_line, 2, sf::Lines);
    }
}

void Game::set_text(sf::Text& text, sf::Font& font, int char_size, sf::Color col, float pos_x, float pos_y) {
    text.setFont(font);
    text.setCharacterSize(char_size);
    text.setFillColor(col);
    text.setPosition(pos_x,pos_y);
}