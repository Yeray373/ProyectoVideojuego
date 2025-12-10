#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Casilla.hpp"
#include "Animacion.hpp"
#include "Pantalla.hpp"

class Enemigo {
protected:  // Cambiar a protected para que las clases hijas accedan
    Casilla* casillaActual;
    sf::Texture EnemigoTexture;
    sf::Sprite EnemigoSprite;
    sf::Clock animationClock;
    bool estaVivo = true;
    bool estaCayendo = false;
    sf::Vector2f posicionCaida;  // Posición inicial hacia donde cae
    sf::Vector2f posicionActualCaida;  // Posición durante la caída
    float velocidadCaida = 50.f;  // Píxeles por segundo
    sf::Clock relojCaida;

    AnimacionEstado estadoActual = AnimacionEstado::PARADO;
    AnimacionEstado estadoAnterior = AnimacionEstado::PARADO;
    std::map<AnimacionEstado, Animacion> animaciones;
    
    // Métodos auxiliares
    void cambiarAnimacion(AnimacionEstado nuevoEstado);
    void actualizarAnimacion();

public:
    Enemigo(Casilla& casillaInicial);
    virtual ~Enemigo();
    
    // Métodos comunes

    void MoverACasilla(Casilla& nuevaCasilla);
    void morir();
    bool getEstaVivo() const { return estaVivo; }
    void Dibujar(Pantalla &window);
    Casilla getCasillaActual();
    
    // Método virtual puro - cada enemigo implementa su comportamiento
    virtual void actualizar(Tablero& tablero) = 0;
};

Enemigo::Enemigo(Casilla& casillaInicial) 
    : casillaActual(&casillaInicial){
}
    
    // Configurar las animaciones usando el constructor de Animacion
  


void Enemigo::MoverACasilla(Casilla& nuevaCasilla)
{
    casillaActual = &nuevaCasilla;
    posicionCaida = nuevaCasilla.getPosicion();
    cambiarAnimacion(AnimacionEstado::SALTAR);  // Activar animación de salto
}

void Enemigo::Dibujar(Pantalla &window)
{
    actualizarAnimacion();
    sf::Vector2f cubePos;

    if (estaCayendo) {

        float tiempocaida = relojCaida.getElapsedTime().asSeconds();
        float desplazamiento = velocidadCaida * tiempocaida;

        posicionActualCaida.x = posicionCaida.x*64.f;
        posicionActualCaida.y = posicionCaida.y + desplazamiento;

        cubePos = posicionActualCaida;
    }else if (casillaActual){

        cubePos = casillaActual->getPosicion();
    }
    EnemigoSprite.setPosition(cubePos.x, cubePos.y);
    window.draw(EnemigoSprite);
    
}

Casilla Enemigo::getCasillaActual()
{
    return *casillaActual;
}

void Enemigo::cambiarAnimacion(AnimacionEstado nuevoEstado)
{
    if (estadoActual != nuevoEstado) {
        estadoAnterior = estadoActual;
        estadoActual = nuevoEstado;
        animaciones[estadoActual].reiniciar();
        animationClock.restart();
    }
}

void Enemigo::actualizarAnimacion()
{
    Animacion& anim = animaciones[estadoActual];
    
    EnemigoSprite.setTextureRect(sf::IntRect(
        anim.getColumnaInicio() + (anim.getFrameActual() * anim.getAnchoFrame()),
        anim.getFilaSprite(),
        anim.getAnchoFrame(),
        anim.getAltoFrame()
    ));
    
    if (animationClock.getElapsedTime().asSeconds() >= anim.getFrameTime()) {
        anim.avanzarFrame();
        
        if (anim.haTerminado()) {
            if(estaVivo == false) {
                cambiarAnimacion(AnimacionEstado::CAER);
            }
            else {
                cambiarAnimacion(AnimacionEstado::PARADO);
            }
        }
        
        animationClock.restart();
    }
}



void Enemigo::morir()
{
    estaVivo = false;
    estaCayendo = true;

    if (casillaActual) {
        posicionActualCaida = casillaActual->getPosicion();
    }
    relojCaida.restart();
    cambiarAnimacion(AnimacionEstado::CAER);
}

Enemigo::~Enemigo()
{
}