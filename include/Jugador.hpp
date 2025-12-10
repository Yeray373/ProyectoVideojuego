#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <Casilla.hpp>
#include <Pantalla.hpp>
#include <Tablero.hpp>
#include <Animacion.hpp>

class Jugador
{
private:
    sf::Texture JugadorTexture;
    sf::Sprite JugadorSprite;
    Casilla* casillaActual;
    Casilla* casillaInicial;  // Casilla de respawn
    sf::Clock animationClock;
    bool estaVivo = true;
    bool estaCayendo = false;
    sf::Vector2f posicionCaida;  // Posición inicial hacia donde cae
    sf::Vector2f posicionActualCaida;  // Posición durante la caída
    float velocidadCaida = 100.f;  // Píxeles por segundo
    sf::Clock relojCaida;
    
    // Estado actual de animación
    AnimacionEstado estadoActual = AnimacionEstado::PARADO;
    AnimacionEstado estadoAnterior = AnimacionEstado::PARADO;
    
    // Mapa de animaciones (defines las coordenadas de cada animación en tu spritesheet)
    std::map<AnimacionEstado, Animacion> animaciones;

public:
    Jugador(Casilla& casillaInicial, const std::string& filePath);
    ~Jugador();
    bool intentarMover(Casilla* nuevaCasilla, int filaDestino, int colDestino);  // Retorna false si murió
    void MoverACasilla(Casilla& nuevaCasilla);
    void morir();
    void respawn();
    bool getEstaVivo() const { return estaVivo; }
    void Dibujar(Pantalla &window);
    Casilla getCasillaActual();
    void cambiarAnimacion(AnimacionEstado nuevoEstado);  // Cambiar animación
    void actualizarAnimacion();  // Actualizar frame de animación
};

Jugador::Jugador(Casilla& casillaInicial, const std::string& filePath) 
    : casillaActual(&casillaInicial), casillaInicial(&casillaInicial)
{
    if (!JugadorTexture.loadFromFile(filePath)) {
        // Error loading texture
    }
    JugadorSprite.setTexture(JugadorTexture);
    JugadorSprite.setScale(0.8f, 0.8f);
    
    // Configurar las animaciones usando el constructor de Animacion
    animaciones[AnimacionEstado::PARADO] = Animacion(101, 5, 1, 74, 151, 0.0f, true);  // 1 frame estático
    animaciones[AnimacionEstado::SALTAR] = Animacion(101, 6, 10, 74, 151, 0.1f, false);  // 3 frames salto
    animaciones[AnimacionEstado::CAER] = Animacion(97, 15, 4, 18, 29, 0.4f, false);    // 5 frames cayendo
    animaciones[AnimacionEstado::DESAPARECER] = Animacion(97, 70, 1, 19, 31, 5.0f, false); // Placeholder
    
    // Establecer animación inicial
    cambiarAnimacion(AnimacionEstado::PARADO);
    casillaActual->CambiarColor();
}

void Jugador::MoverACasilla(Casilla& nuevaCasilla)
{
    casillaActual = &nuevaCasilla;
    casillaActual->CambiarColor();
    cambiarAnimacion(AnimacionEstado::SALTAR);  // Activar animación de salto
}

void Jugador::Dibujar(Pantalla &window)
{
    // Actualizar animación
    actualizarAnimacion();
    
    sf::Vector2f cubePos;
    
    if (estaCayendo) {
        // Actualizar posición de caída (mover hacia abajo gradualmente)
        float tiempoCaida = relojCaida.getElapsedTime().asSeconds();
        float desplazamientoY = velocidadCaida * tiempoCaida;
        
        posicionActualCaida.x = posicionCaida.x;
        posicionActualCaida.y = posicionCaida.y + desplazamientoY;
        
        cubePos = posicionActualCaida;
    } else if (casillaActual) { 
        // Usar la posición de la casilla actual
        cubePos = casillaActual->getPosicion();
    }
    
    JugadorSprite.setPosition(cubePos.x - JugadorSprite.getGlobalBounds().width / 2.f,
                             cubePos.y - JugadorSprite.getGlobalBounds().height + 20.f);
    window.draw(JugadorSprite);
}

Casilla Jugador::getCasillaActual()
{
    return *casillaActual;
}

void Jugador::cambiarAnimacion(AnimacionEstado nuevoEstado)
{
    // Solo cambiar si es diferente
    if (estadoActual != nuevoEstado) {
        estadoAnterior = estadoActual;
        estadoActual = nuevoEstado;
        animaciones[estadoActual].reiniciar();  // Usar método de la clase
        animationClock.restart();
    }
}

void Jugador::actualizarAnimacion()
{
    Animacion& anim = animaciones[estadoActual];
    
    // Siempre actualizar el sprite con el frame actual
    JugadorSprite.setTextureRect(sf::IntRect(
        anim.getColumnaInicio() + (anim.getFrameActual() * anim.getAnchoFrame()),
        anim.getFilaSprite(),
        anim.getAnchoFrame(),
        anim.getAltoFrame()
    ));
    
    // Actualizar frame si ha pasado suficiente tiempo
    if (animationClock.getElapsedTime().asSeconds() >= anim.getFrameTime()) {
        // Avanzar al siguiente frame
        anim.avanzarFrame();
        
        // Manejar fin de animación
        if (anim.haTerminado()) {
            if(estaVivo == false) {
                cambiarAnimacion(AnimacionEstado::DESAPARECER);
            }
            else {
                cambiarAnimacion(AnimacionEstado::PARADO);  // Volver a parado
            }
        }
        
        animationClock.restart();
    }
}

bool Jugador::intentarMover(Casilla* nuevaCasilla, int filaDestino, int colDestino)
{
    if (nuevaCasilla == nullptr) {
        // Calcular posición visual de caída (extrapolando desde la posición actual)
        float x = (colDestino * 64.f) - (filaDestino * 64.f / 2.f);
        float y = (filaDestino * 45.f);
        posicionCaida = sf::Vector2f(450.f + x, 80.f + y);
        
        // El jugador intentó moverse a una casilla inválida
        morir();
        return false;
    }
    
    MoverACasilla(*nuevaCasilla);
    return true;
}

void Jugador::morir()
{
    estaVivo = false;
    estaCayendo = true;
    // Inicializar posición de caída desde la posición actual
    if (casillaActual) {
        posicionActualCaida = casillaActual->getPosicion();
    }
    relojCaida.restart();
    cambiarAnimacion(AnimacionEstado::CAER);
    // Puedes agregar efectos de sonido, partículas, etc.
}

void Jugador::respawn()
{
    estaVivo = true;
    estaCayendo = false;
    casillaActual = casillaInicial;
    cambiarAnimacion(AnimacionEstado::PARADO);
}

Jugador::~Jugador()
{
    // Destructor
}
