#pragma once
#include <SFML/Graphics.hpp>
#include <Pantalla.hpp>
class Background
{
private:
    sf::Texture BackgroundImage;
    sf::Sprite background;
public:
    Background(const std::string& filePath);
    void Dibujar(Pantalla &window);
};

Background::Background(const std::string& filePath)
{
    if (!BackgroundImage.loadFromFile(filePath)) {}
    background.setTexture(BackgroundImage);
    background.setScale(1.f, 1.f);
}

void Background::Dibujar(Pantalla &window)
{
    window.draw(background);
}