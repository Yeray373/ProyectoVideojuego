#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <Casilla.hpp>
#include <Animacion.hpp>
#include <Pantalla.hpp>

class Enemigo {
protected:  // Cambiar a protected para que las clases hijas accedan
    Casilla* casillaActual;
    sf::Texture EnemigoTexture;
    sf::Sprite EnemigoSprite;
    sf::Clock animationClock;
    bool estaVivo = true;
    bool estaCayendo = false;
    bool estaMoviendose = false;  // Nueva: indica si está en transición entre casillas
    sf::Vector2f posicionInicio;  // Nueva: posición de inicio del movimiento
    sf::Vector2f posicionDestino; // Nueva: posición de destino del movimiento
    sf::Clock relojMovimiento;    // Nueva: para controlar la duración del movimiento
    float duracionMovimiento = 1.f; // Nueva: duración de la transición en segundos
    Casilla* casillaSiguiente = nullptr; // Nueva: casilla a la que se moverá
    
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
    bool intentarMover(Casilla* nuevaCasilla);
    void MoverACasilla(Casilla& nuevaCasilla);
    void morir();
    bool getEstaVivo() const { return estaVivo; }
    bool debeSerEliminado() const;  // Nueva: verifica si debe eliminarse
    void Dibujar(Pantalla &window);
    Casilla* getCasillaActual();
    
    // Método virtual puro - cada enemigo implementa su comportamiento
    virtual void actualizar(Tablero& tablero) = 0;
};

Enemigo::Enemigo(Casilla& casillaInicial) 
    : casillaActual(&casillaInicial){
}
    
    // Configurar las animaciones usando el constructor de Animacion
  


bool Enemigo::intentarMover(Casilla* nuevaCasilla)
{
    if (nuevaCasilla == nullptr) {
        morir();
        return false;
    }
    
    MoverACasilla(*nuevaCasilla);
    return true;
}

void Enemigo::MoverACasilla(Casilla& nuevaCasilla)
{   
    // Iniciar transición animada
    estaMoviendose = true;
    casillaSiguiente = &nuevaCasilla;
    posicionInicio = casillaActual->getPosicion();
    posicionDestino = nuevaCasilla.getPosicion();
    relojMovimiento.restart();
    cambiarAnimacion(AnimacionEstado::SALTAR);
}

void Enemigo::Dibujar(Pantalla &window)
{
    actualizarAnimacion();
    sf::Vector2f cubePos;

    if (estaCayendo) {
        float tiempocaida = relojCaida.getElapsedTime().asSeconds();
        float desplazamiento = velocidadCaida * tiempocaida;
        posicionActualCaida.x = posicionCaida.x;
        posicionActualCaida.y = posicionCaida.y + desplazamiento;
        cubePos = posicionActualCaida;
    } else if (estaMoviendose) {
        
        float tiempoTranscurrido = relojMovimiento.getElapsedTime().asSeconds();
        float progreso = tiempoTranscurrido / duracionMovimiento;
        
        if (progreso >= 1.0f) {
            // Movimiento completado
            progreso = 1.0f;
            estaMoviendose = false;
            casillaActual = casillaSiguiente;
            casillaSiguiente = nullptr;
            cambiarAnimacion(AnimacionEstado::PARADO);
        }
        
        
        cubePos.x = posicionInicio.x + (posicionDestino.x - posicionInicio.x) * progreso;
        cubePos.y = posicionInicio.y + (posicionDestino.y - posicionInicio.y) * progreso;
    } else if (casillaActual) {
        cubePos = casillaActual->getPosicion();
    }
    
    // Centrar y ajustar el sprite igual que el jugador
    EnemigoSprite.setPosition(cubePos.x - EnemigoSprite.getGlobalBounds().width / 2.f,
                             cubePos.y - EnemigoSprite.getGlobalBounds().height + 20.f);
    window.draw(EnemigoSprite);
}

Casilla* Enemigo::getCasillaActual()
{
    return casillaActual;
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
        posicionCaida = casillaActual->getPosicion();
        posicionActualCaida = posicionCaida;
    }
    
    // Cambiar inmediatamente a animación de caer, interrumpiendo cualquier animación actual
    estadoActual = AnimacionEstado::CAER;
    animaciones[AnimacionEstado::CAER].reiniciar();
    animationClock.restart();
    
    relojCaida.restart();
}

bool Enemigo::debeSerEliminado() const
{
    // Eliminar si está cayendo y ha caído más de 500 píxeles
    if (estaCayendo) {
        float tiempoCaida = relojCaida.getElapsedTime().asSeconds();
        float desplazamiento = velocidadCaida * tiempoCaida;
        return desplazamiento > 500.f;  // Ha caído fuera de la pantalla
    }
    return false;
}

Enemigo::~Enemigo()
{
}