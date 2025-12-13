#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <Pantalla.hpp>

class Casilla{
private:
    const float ancho_cubo = 64.f;
    const float profundidad_cubo = 32.f;
    const float alto_cubo = 32.f; 
    int fila;
    int columna;
public:
    sf::ConvexShape tapa_cubo,izq_cubo,der_cubo;
    
    bool visitada = false;

    sf::Color colorOriginal = sf::Color::Blue;
    sf::Color colorVisitado = sf::Color::Yellow;
    
    Casilla (float x, float y, int fila, int columna);

    void Dibujar(Pantalla &window){
        window.draw(tapa_cubo);
        window.draw(der_cubo);
        window.draw(izq_cubo);
    }
    
    bool CambiarColor() {
        if (!visitada) {
            visitada = true;
            tapa_cubo.setFillColor(colorVisitado);
            return true; // Retorna true si cambió el color
        }
        return false; // Ya estaba visitada
    }
    
    bool RestaurarColor() {
        if (visitada) {
            visitada = false;
            tapa_cubo.setFillColor(colorOriginal);
            return true; // Retorna true si restauró el color
        }
        return false; // Ya estaba sin visitar
    }
    
    sf::Vector2f getPosicion() {
        return tapa_cubo.getPoint(0);
    }
    int getFila() const {
        return fila;
    }
    int getColumna() const {
        return columna;
    }

};

Casilla::Casilla(float x, float y, int fila, int columna) : fila(fila), columna(columna) {
    //Tapa
    tapa_cubo.setPointCount(4);
    tapa_cubo.setPoint(0, sf::Vector2f(x, y)); // Arriba
    tapa_cubo.setPoint(1, sf::Vector2f(x + ancho_cubo / 2.f, y + alto_cubo / 2.f)); // Derecha
    tapa_cubo.setPoint(2, sf::Vector2f(x, y+profundidad_cubo));//Abajo
    tapa_cubo.setPoint(3, sf::Vector2f(x - ancho_cubo / 2.f, y + alto_cubo / 2.f)); // Izquierda
    tapa_cubo.setFillColor(sf::Color::Blue);
    sf::Vector2f posdown = tapa_cubo.getPoint(2);
    sf::Vector2f posright = tapa_cubo.getPoint(1);
    sf::Vector2f posleft = tapa_cubo.getPoint(3);
    //Lado derecho
    der_cubo.setPointCount(4);
    der_cubo.setPoint(0, posright);
    der_cubo.setPoint(1, posdown);
    der_cubo.setPoint(2, sf::Vector2f(posdown.x, posdown.y + alto_cubo));
    der_cubo.setPoint(3, sf::Vector2f(posright.x, posright.y + alto_cubo));
    der_cubo.setFillColor(sf::Color::Green);
    //Lado izquierdo
    izq_cubo.setPointCount(4);
    izq_cubo.setPoint(0, posdown);
    izq_cubo.setPoint(1, posleft);
    izq_cubo.setPoint(2, sf::Vector2f(posleft.x, posleft.y + alto_cubo));
    izq_cubo.setPoint(3, sf::Vector2f(posdown.x, posdown.y + alto_cubo));
    izq_cubo.setFillColor(sf::Color::Red);//un saludo
}
