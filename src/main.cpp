#include <SFML/Graphics.hpp>

int main()
{

    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Timber");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }
            // if (event->is<sf::Event::Closed>())
            // {
            //     window.close();
            // }
        }
        window.clear();
        window.display();
    }
    return 0;
}
