#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Rock {
    sf::RectangleShape shape;

    Rock(float x, float y, float size) {
        shape.setSize(sf::Vector2f(size, size));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
    }
};

bool checkCollision(const sf::RectangleShape& square, const Rock& rock) {
    return square.getGlobalBounds().intersects(rock.shape.getGlobalBounds());
}

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Moving Square");

    // Create a square shape
    sf::RectangleShape square(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Green);

    // Set the initial position of the square
    float posX = 375.0f;
    float posY = 275.0f;
    square.setPosition(posX, posY);

    // Set the speed of the square movement
    float speed = 0.01f;

    // Seed for random terrain generation
    std::srand(std::time(0));

    // Vector to store rocks
    std::vector<Rock> rocks;

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

        // Move the square based on keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && posX > 0) {
            bool canMove = true;
            for (const auto& rock : rocks) {
                if (checkCollision(square, rock)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) posX -= speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && posX < playableAreaSize.x - square.getSize().x) {
            bool canMove = true;
            for (const auto& rock : rocks) {
                if (checkCollision(square, rock)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) posX += speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && posY > 0) {
            bool canMove = true;
            for (const auto& rock : rocks) {
                if (checkCollision(square, rock)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) posY -= speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && posY < playableAreaSize.y - square.getSize().y) {
            bool canMove = true;
            for (const auto& rock : rocks) {
                if (checkCollision(square, rock)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) posY += speed;
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
            for (int i = 0; i < 5; ++i) {
                rocks.emplace_back(
                    std::rand() % static_cast<int>(playableAreaSize.x),
                    std::rand() % static_cast<int>(playableAreaSize.y),
                    20.0f
                );
            }
        }

        // Update the position of the square
        square.setPosition(posX, posY);

        // Clear the window
        window.clear();

        // Draw the rocks
        for (const auto& rock : rocks) {
            window.draw(rock.shape);
        }

        // Draw the square
        window.draw(square);

        // Display the contents of the window
        window.display();
    }

    return 0;
}

