#include "StateManager.h"

class MainMenuState : public State {
    int selectedIndex = 0;
    std::vector<std::string> options = {"Start Simulation", "Exit"};
    sf::Text text;
public:
    void handleInput(GameEngine& game, sf::Event event) override;
    void update(GameEngine& game, float dt) override {}
    void draw(GameEngine& game, sf::RenderWindow& window) override;
};

class SimulationState : public State {
    Map map;
    std::vector<Agent*> agents;
    QBrain redBrain, blueBrain;
public:
    SimulationState() {
        map.generate(time(NULL));
        for(int i=0; i<5; ++i) agents.push_back(new Soldier(Team::RED, map.getGridPosition(2, 2+i), &redBrain));
        agents.push_back(new Medic(Team::RED, map.getGridPosition(1, 5), &redBrain));
        for(int i=0; i<5; ++i) agents.push_back(new Soldier(Team::BLUE, map.getGridPosition(GRID_WIDTH-3, 2+i), &blueBrain));
        agents.push_back(new Sniper(Team::BLUE, map.getGridPosition(GRID_WIDTH-2, 10), &blueBrain));
    }
    ~SimulationState() { for(auto* a : agents) delete a; }
    void handleInput(GameEngine& game, sf::Event event) override {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) game.changeState(std::make_unique<MainMenuState>());
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) game.changeState(std::make_unique<SimulationState>());
    }
    void update(GameEngine& game, float dt) override { for(auto* agent : agents) agent->update(map, agents, dt); }
    void draw(GameEngine& game, sf::RenderWindow& window) override {
        map.draw(window);
        for(auto* agent : agents) agent->draw(window);
        sf::Text info("Press ESC to Menu, R to Reset", game.font, 20); info.setPosition(10, 10); window.draw(info);
    }
};

void MainMenuState::handleInput(GameEngine& game, sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
        if (event.key.code == sf::Keyboard::Down) selectedIndex = (selectedIndex + 1) % options.size();
        if (event.key.code == sf::Keyboard::Enter) {
            if (selectedIndex == 0) game.changeState(std::make_unique<SimulationState>());
            if (selectedIndex == 1) game.window.close();
        }
    }
}

void MainMenuState::draw(GameEngine& game, sf::RenderWindow& window) {
    text.setFont(game.font); text.setCharacterSize(40);
    float y = 200;
    for (size_t i = 0; i < options.size(); ++i) {
        text.setString(options[i]);
        text.setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition((WINDOW_WIDTH - bounds.width) / 2, y);
        window.draw(text);
        y += 60;
    }
    sf::Text title("WAR SIMULATION AI", game.font, 60);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition((WINDOW_WIDTH - title.getLocalBounds().width)/2, 50);
    window.draw(title);
}

GameEngine::GameEngine() {
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "WarSim C++20");
    window.setFramerateLimit(60);
    // Versuche Font zu kopieren falls nicht da
    if(!font.loadFromFile("arial.ttf")) {
        if(!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) std::cerr << "FONT NOT FOUND" << std::endl;
    }
    changeState(std::make_unique<MainMenuState>());
}
void GameEngine::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (currentState) currentState->handleInput(*this, event);
        }
        if (currentState) currentState->update(*this, dt.asSeconds());
        window.clear(sf::Color::Black);
        if (currentState) currentState->draw(*this, window);
        window.display();
    }
    currentState.reset(); // Cleanup
    window.close();
}

void GameEngine::changeState(std::unique_ptr<State> newState) { currentState = std::move(newState); }
