#include <iostream>
#include <fstream>

#include <SFML/Window.hpp>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.h"
#include "Object.h"
#include "Player.h"

int main()
{

    int winWidth = 1500;
    int winHeight = 1000;

    // create the window
    sf::Window window(sf::VideoMode(winWidth, winHeight), "OpenGL", sf::Style::Default, sf::ContextSettings(24, 8, 2, 3, 3));
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    sf::Mouse::setPosition(sf::Vector2i(winWidth / 2, winHeight / 2), window);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    glewInit();

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");

    Player player(winWidth, winHeight);

    Object box("resources/models/cube/cube.obj", &shader);
    Object ground("resources/models/ground/ground.obj", &shader);
    
    sf::Clock clock;

    bool escPressed = false;

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                winWidth = event.size.width;
                winHeight = event.size.height;
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        mousePos = player.updateMouse(mousePos);
        sf::Mouse::setPosition(mousePos, window);

        float elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();

        player.setElapsedTime(elapsedTime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.forward();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.backward();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.strafeLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.strafeRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            if (!escPressed) {
                escPressed = true;
                if (!camera.getEsc()) {
                    window.setMouseCursorVisible(true);
                    window.setMouseCursorGrabbed(false);
                    camera.setEsc(true);
                }
                else {
                    window.setMouseCursorVisible(false);
                    window.setMouseCursorGrabbed(true);
                    camera.setEsc(false);
                    sf::Mouse::setPosition(sf::Vector2i(winWidth / 2, winHeight / 2), window);
                }
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            escPressed = false;
        }

        // clear the buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...
        shader.use();

        camera.look(shader);

        ground.draw();
        box.draw();

        window.display();
    }

    return 0;
}