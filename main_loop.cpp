#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

struct Rock {
    sf::RectangleShape shape;
    sf::Clock projectile_timer;

    Rock(float x, float y, float size) {
        shape.setSize(sf::Vector2f(size, size));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
    }
};

struct Projectile {
  sf::RectangleShape shape;
  sf::Vector2f velocity;

  Projectile(float x, float y, float size, sf::Vector2f vel) : velocity(vel) {
    shape.setSize(sf::Vector2f(size, size));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue);
  }
};

bool checkCollision(const sf::RectangleShape& square, const sf::RectangleShape& other) {
    return square.getGlobalBounds().intersects(other.getGlobalBounds());
}

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Moving Square");

    // Create a square shape
    int squareSize = 20;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setFillColor(sf::Color::Green);

    // Set the initial position of the square
    float posX = 375.0f;
    float posY = 275.0f;
    square.setPosition(posX, posY);

    // Set the speed of the square movement
    float squareSpeed = 0.01f;
    std::srand(std::time(0));

    // Game Items
    std::vector<Rock> rocks;
    std::vector<Projectile> projectiles;

    // Initialize playable area size
    sf::Vector2f playableAreaSize(window.getSize().x, window.getSize().y);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;

        // Event handling
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (auto it = rocks.begin(); it != rocks.end();) {
            if (checkCollision(square, it->shape)) {
                it = rocks.erase(it);
            } else {
                ++it;
            }
        }

        // Move the square based on keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && posX > 0) {
            posX -= squareSpeed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && posX < playableAreaSize.x - square.getSize().x) {
            posX += squareSpeed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && posY > 0) {
            posY -= squareSpeed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && posY < playableAreaSize.y - square.getSize().y) {
            posY += squareSpeed;
        }

        // Check if the square is at the edge of the playable area
        bool atEdgeX = posX <= 0 || posX >= playableAreaSize.x - square.getSize().x;
        bool atEdgeY = posY <= 0 || posY >= playableAreaSize.y - square.getSize().y;

        if (atEdgeX || atEdgeY) {
            // Move the square to the opposite edge
            if (atEdgeX) {
                posX = (posX <= 0) ? (playableAreaSize.x - square.getSize().x) : 0;
            }

            if (atEdgeY) {
                posY = (posY <= 0) ? (playableAreaSize.y - square.getSize().y) : 0;
            }

            // Generate new rocks for the area
            rocks.clear();
            float rockSize = 20.0f;
            int numberOfRocks = 5;
            for (int i = 0; i < numberOfRocks; ++i) {
                rocks.emplace_back(
                    std::rand() % static_cast<int>(playableAreaSize.x),
                    std::rand() % static_cast<int>(playableAreaSize.y),
                    rockSize
                );
            }
        }

        square.setPosition(posX, posY);
        window.clear();

        for (const auto& rock : rocks) {
            window.draw(rock.shape);
        }

        for (auto& rock : rocks) {
            float fireInterval = 1.0f;
            sf::Time interval = sf::seconds(fireInterval);

            if (rock.projectile_timer.getElapsedTime() > interval) {
                
                float angle = static_cast<float>(std::rand() % 360);
                sf::Vector2f direction(std::cos(angle), std::sin(angle));

                float projectileSize = 5.0f;
                float speed = 0.01f; 

                projectiles.emplace_back(
                    rock.shape.getPosition().x,
                    rock.shape.getPosition().y,
                    projectileSize,
                    direction * speed
                );

                rock.projectile_timer.restart();
            }
        }
        
        for (auto& projectile : projectiles) {
            projectile.shape.move(projectile.velocity);

            if (checkCollision(square, projectile.shape)) {
                sf::Vector2u windowSize = window.getSize();

                float middleX = static_cast<float>(windowSize.x) / 2.0f;
                float middleY = static_cast<float>(windowSize.y) / 2.0f;

                posX = middleX;
                posY = middleY;
            }
        }

        projectiles.erase(
            std::remove_if(
                projectiles.begin(),
                projectiles.end(),
                [&](const Projectile& projectile) {
                    return (
                        projectile.shape.getPosition().x < 0 ||
                        projectile.shape.getPosition().x > window.getSize().x ||
                        projectile.shape.getPosition().y < 0 ||
                        projectile.shape.getPosition().y > window.getSize().y
                    );
                }
            ),
            projectiles.end()
        );


        for(const auto& projectile : projectiles) {
            window.draw(projectile.shape);
        }


        // Draw the square
        window.draw(square);

        // Display the contents of the window
        window.display();
    }

    return 0;
}

