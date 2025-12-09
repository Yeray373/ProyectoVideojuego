#pragma once
#include <SFML/Graphics.hpp>

class Pantalla
{
private:
    sf::RenderWindow window;
    const float Ancho = 900.f;
    const float Alto = 600.f;
    float frameTime = 0.1f;
    int currentFrame = 0;
    int numFrames = 4; // Número total de frames en la animación

public:
    Pantalla();
    ~Pantalla();
    bool isOpen();
    void close();
    void clear();
    void display();
    bool pollEvent(sf::Event &event);
    void draw(sf::Sprite &sprite);
    void draw(sf::Shape &shape);
    void draw(sf::Drawable &drawable);
    void draw(sf::Text &text);
    float getFrameTime() ;
    int getCurrentFrame() ;
    int getNumFrames() ;
    void setCurrentFrame(int frame);
};

Pantalla::Pantalla()
{
    window.create(sf::VideoMode(Ancho, Alto), "Sobreviviendo al CETI");
}

bool Pantalla::isOpen()
{
    return window.isOpen();
}
void Pantalla::close()
{
    window.close();
}
void Pantalla::clear()
{
    window.clear();
}
void Pantalla::display()
{
    window.display();
}
bool Pantalla::pollEvent(sf::Event &event)
{
    return window.pollEvent(event);
}

// Implementación de métodos draw
void Pantalla::draw(sf::Sprite &sprite)
{
    window.draw(sprite);
}

void Pantalla::draw(sf::Shape &shape)
{
    window.draw(shape);
}

void Pantalla::draw(sf::Drawable &drawable)
{
    window.draw(drawable);
}

void Pantalla::draw(sf::Text &text)
{
    window.draw(text);
}

float Pantalla::getFrameTime()  {
    return frameTime;
}

int Pantalla::getCurrentFrame()  {
    return currentFrame;
}

int Pantalla::getNumFrames()  {
    return numFrames;
}

void Pantalla::setCurrentFrame(int frame) {
    currentFrame = frame % numFrames;
}

Pantalla::~Pantalla()
{
}
