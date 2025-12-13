#pragma once
#include <Enemigo.hpp>
#include <Tablero.hpp>
#include <cstdlib>

class Enemigo_Profesor : public Enemigo {
private:
    sf::Clock relojMovimiento;
    float tiempoEntreMovimientos = 2.5f;  // Se mueve cada 2 segundos

public:
    Enemigo_Profesor(Casilla& casillaInicial, const std::string& filePath): Enemigo(casillaInicial) {
        if(!EnemigoTexture.loadFromFile(filePath)) {
        }
        EnemigoSprite.setTexture(EnemigoTexture);
        EnemigoSprite.setScale(0.8f, 0.8f);
        animaciones[AnimacionEstado::PARADO] = Animacion(232, 0, 1, 114, 190, 0.0f, true);  // 1 frame estático
        animaciones[AnimacionEstado::SALTAR] = Animacion(232, 0, 7, 114, 190, 0.25f, false);  // 3 frames salto
        animaciones[AnimacionEstado::CAER] = Animacion(27, 0, 3, 114, 150, 0.7f, true);  // 5 frames morir
         // Establecer animación inicial
        cambiarAnimacion(AnimacionEstado::PARADO);
    }
    
    void actualizar(Tablero& tablero) override {
        if (!estaVivo || estaCayendo || estaMoviendose) return;
        
        
        if (relojMovimiento.getElapsedTime().asSeconds() >= tiempoEntreMovimientos) {
            
            int filaActual = casillaActual->getFila();
            int colActual = casillaActual->getColumna();
            
            // Elegir dirección aleatoria (abajo-izquierda o abajo-derecha)
            int direccion = std::rand() % 2;
            int filaDestino, colDestino;
            
            if (direccion == 0) {
                // Abajo-izquierda
                filaDestino = filaActual + 1;
                colDestino = colActual;
            } else {
                // Abajo-derecha
                filaDestino = filaActual + 1;
                colDestino = colActual + 1;
            }
            
            // Intentar mover
            Casilla* nuevaCasilla = tablero.getCasilla(filaDestino, colDestino);
            intentarMover(nuevaCasilla);
            
            relojMovimiento.restart();
        }
    }
    
    ~Enemigo_Profesor() override = default;
};
