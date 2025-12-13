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
    void clear(sf::Color color);
    void display();
    bool pollEvent(sf::Event &event);
    void draw(const sf::Sprite &sprite);
    void draw(const sf::Shape &shape);
    void draw(const sf::Drawable &drawable);
    void draw(const sf::Text &text);
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
void Pantalla::clear(sf::Color color)
{
    window.clear(color);
}
void Pantalla::display()
{
    window.display();
}
bool Pantalla::pollEvent(sf::Event &event)
{
    return window.pollEvent(event);
}


void Pantalla::draw(const sf::Sprite &sprite)
{
    window.draw(sprite);
}

void Pantalla::draw(const sf::Shape &shape)
{
    window.draw(shape);
}

void Pantalla::draw(const sf::Drawable &drawable)
{
    window.draw(drawable);
}

void Pantalla::draw(const sf::Text &text)
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
