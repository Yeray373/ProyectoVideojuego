#pragma once
#include "Enemigo.hpp"
#include "Tablero.hpp"
#include <cstdlib>

class Enemigo_Integral : public Enemigo {
private:
    sf::Clock relojMovimiento;
    float tiempoEntreMovimientos = 3.0f;  // Más rápido que el profesor
    bool moviendoAbajo = true;  // Patrón: zigzag
    bool restauroColorEnUltimoMovimiento = false; //tracking de restauración
    Casilla* casillaAnterior = nullptr; // memoria de última casilla visitada
    int contadorMovimientos = 0; // Contador para restaurar cada 2 movimientos

public:
    Enemigo_Integral(Casilla& casillaInicial, const std::string& filePath)
        : Enemigo(casillaInicial) {
            if(!EnemigoTexture.loadFromFile(filePath)) {
            }
            EnemigoSprite.setTexture(EnemigoTexture);
            EnemigoSprite.setScale(0.8f, 0.8f);
            animaciones[AnimacionEstado::PARADO] = Animacion(24, 0, 1, 95, 212, 0.0f, true);  // 1 frame estático
            animaciones[AnimacionEstado::SALTAR] = Animacion(24, 0, 5, 95, 212, 0.25f, false);  // 3 frames salto
            //animaciones[AnimacionEstado::CAER] = Animacion(27, 0, 3, 114, 150, 0.2f, true);  // 5 frames morir

    // Establecer animación inicial
            cambiarAnimacion(AnimacionEstado::PARADO);
        }

    
    void actualizar(Tablero& tablero) override {
        if (!estaVivo || estaCayendo || estaMoviendose) return;
        
        // Movimiento(patrón zigzag)
        if (relojMovimiento.getElapsedTime().asSeconds() >= tiempoEntreMovimientos) {
            restauroColorEnUltimoMovimiento = false; // Reset antes del movimiento
            
            int filaActual = casillaActual->getFila();
            int colActual = casillaActual->getColumna();
            
            // múltiples direcciones de movimiento
            std::vector<std::pair<int, int>> direccionesPosibles;
            
            if (moviendoAbajo) {
                // Prioridad a bajar
                direccionesPosibles.push_back({filaActual + 1, colActual});     // Abajo-izquierda
                direccionesPosibles.push_back({filaActual + 1, colActual + 1}); // Abajo-derecha
                direccionesPosibles.push_back({filaActual - 1, colActual});     // Arriba-izquierda
                direccionesPosibles.push_back({filaActual - 1, colActual - 1}); // Arriba-derecha
            } else {
                // Prioridad a subir
                direccionesPosibles.push_back({filaActual - 1, colActual});     // Arriba-izquierda
                direccionesPosibles.push_back({filaActual - 1, colActual - 1}); // Arriba-derecha
                direccionesPosibles.push_back({filaActual + 1, colActual});     // Abajo-izquierda
                direccionesPosibles.push_back({filaActual + 1, colActual + 1}); // Abajo-derecha
            }
            
            // Intentar cada dirección hasta encontrar una válida
            bool seMovio = false;
            for (const auto& dir : direccionesPosibles) {
                Casilla* nuevaCasilla = tablero.getCasilla(dir.first, dir.second);
                // No volver a la casilla donde estaba antes (evitar oscilación)
                if (nuevaCasilla != nullptr && nuevaCasilla != casillaAnterior) {
                    // Incrementar contador de movimientos
                    contadorMovimientos++;
                    
                    // Solo restaurar el color cada 2 movimientos
                    if (contadorMovimientos >= 2) {
                        restauroColorEnUltimoMovimiento = nuevaCasilla->RestaurarColor();
                        contadorMovimientos = 0; // Resetear contador
                    } else {
                        restauroColorEnUltimoMovimiento = false;
                    }
                    
                    casillaAnterior = casillaActual; // Guardar casilla actual antes de moverse
                    intentarMover(nuevaCasilla);
                    seMovio = true;
                    break;
                }
            }
            
            // Si no pudo moverse en ninguna dirección (todas bloqueadas o son la anterior), 
            // resetear memoria y cambiar patrón
            if (!seMovio) {
                casillaAnterior = nullptr;
                moviendoAbajo = !moviendoAbajo;
            }
            
            relojMovimiento.restart();
        }
    }
    
    bool restauroColor() {
        bool resultado = restauroColorEnUltimoMovimiento;
        restauroColorEnUltimoMovimiento = false; // Reset después de verificar
        return resultado;
    }
    
    ~Enemigo_Integral() override = default;
};
