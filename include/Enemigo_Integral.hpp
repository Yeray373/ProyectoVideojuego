#pragma once
#include "Enemigo.hpp"
#include "Tablero.hpp"
#include <cstdlib>

class Enemigo_Integral : public Enemigo {
private:
    sf::Clock relojMovimiento;
    float tiempoEntreMovimientos = 1.0f;  // Más rápido que el profesor
    bool moviendoAbajo = true;  // Patrón: zigzag

public:
    Enemigo_Integral(Casilla& casillaInicial, const std::string& filePath)
        : Enemigo(casillaInicial) {
            if(!EnemigoTexture.loadFromFile(filePath)) {
            }
            EnemigoSprite.setTexture(EnemigoTexture);
            EnemigoSprite.setScale(0.8f, 0.8f);
            animaciones[AnimacionEstado::PARADO] = Animacion(24, 0, 1, 95, 212, 0.0f, true);  // 1 frame estático
            animaciones[AnimacionEstado::SALTAR] = Animacion(24, 0, 5, 95, 212, 0.2f, false);  // 3 frames salto
            //animaciones[AnimacionEstado::CAER] = Animacion(27, 0, 3, 114, 150, 0.2f, true);  // 5 frames morir

    // Establecer animación inicial
            cambiarAnimacion(AnimacionEstado::PARADO);
        }

    
    void actualizar(Tablero& tablero) override {
        if (!estaVivo || estaCayendo) return;
        
        // Movimiento más frecuente (patrón zigzag)
        if (relojMovimiento.getElapsedTime().asSeconds() >= tiempoEntreMovimientos) {
            int filaActual = casillaActual->getFila();
            int colActual = casillaActual->getColumna();
            
            int filaDestino, colDestino;
            
            if (moviendoAbajo) {
                // Moverse hacia abajo alternando izquierda/derecha
                filaDestino = filaActual + 1;
                // Alternar entre izquierda y derecha
                colDestino = (std::rand() % 2 == 0) ? colActual : colActual + 1;
            } else {
                // Moverse hacia arriba (ocasionalmente)
                filaDestino = filaActual - 1;
                colDestino = colActual;
            }
            
            // Intentar mover
            Casilla* nuevaCasilla = tablero.getCasilla(filaDestino, colDestino);
            
            if (nuevaCasilla == nullptr) {
                // Si no puede moverse en esa dirección, cambiar patrón
                moviendoAbajo = !moviendoAbajo;
            } else {
                MoverACasilla(*nuevaCasilla);
            }
            
            relojMovimiento.restart();
        }
    }
    
    ~Enemigo_Integral() override = default;
};
