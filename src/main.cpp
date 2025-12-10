#include <SFML/Graphics.hpp>
#include <Pantalla.hpp>
#include <Jugador.hpp>
#include <Casilla.hpp>
#include <Background.hpp>
#include <iostream>
#include <vector>
#include <Tablero.hpp>




int main() {
    Pantalla gameWindow; // Inicializa la ventana del juego
    Background background("./assets/images/Background.jpg"); // Carga el fondo del juego
    Tablero tablero(7); // Crea un tablero con una pirámide de altura 7
    Jugador jugador(     // Inicializa el jugador en la primera casilla (vértice)
        tablero.casillas[0][0], 
        "./assets/images/Oldhero.png"
    );   

    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }

            int filaActual = jugador.getCasillaActual().getFila();
            int columnaActual = jugador.getCasillaActual().getColumna();

            if (event.type == sf::Event::KeyPressed) {
                if (!jugador.getEstaVivo()) {
                    // Si el jugador está muerto, presionar cualquier tecla lo respawnea
                    if (event.key.code == sf::Keyboard::Space) {
                        jugador.respawn();
                    }
                } else {
                    // Movimiento diagonal abajo-derecha (Down + Right)
                    if (event.key.code == sf::Keyboard::Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        int filaDestino = filaActual + 1;
                        int colDestino = columnaActual + 1;
                        Casilla* nuevaCasilla = tablero.getCasilla(filaDestino, colDestino);
                        jugador.intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                    // Movimiento diagonal abajo-izquierda (Down + Left)
                    else if (event.key.code == sf::Keyboard::Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        int filaDestino = filaActual + 1;
                        int colDestino = columnaActual;
                        Casilla* nuevaCasilla = tablero.getCasilla(filaDestino, colDestino);
                        jugador.intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                    // Movimiento diagonal arriba-derecha (Up + Right)
                    else if (event.key.code == sf::Keyboard::Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        int filaDestino = filaActual - 1;
                        int colDestino = columnaActual;
                        Casilla* nuevaCasilla = tablero.getCasilla(filaDestino, colDestino);
                        jugador.intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                    // Movimiento diagonal arriba-izquierda (Up + Left)
                    else if (event.key.code == sf::Keyboard::Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        int filaDestino = filaActual - 1;
                        int colDestino = columnaActual - 1;
                        Casilla* nuevaCasilla = tablero.getCasilla(filaDestino, colDestino);
                        jugador.intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                }
            }
        }
        gameWindow.clear();
        background.Dibujar(gameWindow);  // Dibujar background primero
        tablero.Dibujar(gameWindow);
        jugador.Dibujar(gameWindow);
        gameWindow.display();
        //Arthur estuvo aqui
        // ok
        
    }

    return 0;
}

