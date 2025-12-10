#include <SFML/Graphics.hpp>
#include <Pantalla.hpp>
#include <Jugador.hpp>
#include <Enemigo.hpp>
#include <Enemigo_Profesor.hpp>
#include <Enemigo_Integral.hpp>
#include <Casilla.hpp>
#include <Background.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <Tablero.hpp>




int main() {
    // Inicializar generador de números aleatorios
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    Pantalla gameWindow; // Inicializa la ventana del juego
    Background background("./assets/images/Background.png"); // Carga el fondo del juego
    Tablero tablero(7); // Crea un tablero con una pirámide de altura 7
    Jugador jugador(     // Inicializa el jugador en la primera casilla (vértice)
        tablero.casillas[0][0], 
        "./assets/images/cetiano.png"
    );
    
    // Sistema de generación de enemigos (polimórfico)
    std::vector<std::unique_ptr<Enemigo>> enemigos;
    sf::Clock relojSpawn;
    float tiempoProximoSpawn = 2.0f + static_cast<float>(std::rand() % 4); // Entre 2-6 segundos
    int maxEnemigos = 2; // Máximo de enemigos simultáneos


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
        
        // Sistema de spawn de enemigos
        if (relojSpawn.getElapsedTime().asSeconds() >= tiempoProximoSpawn && enemigos.size() < maxEnemigos) {
            // Generar fila y columna aleatoria
            int filaAleatoria = std::rand() % tablero.getAltura();
            int columnaAleatoria = std::rand() % (filaAleatoria + 1); // Las columnas van de 0 a fila
            
            // Obtener la casilla
            Casilla* casillaSpawn = tablero.getCasilla(filaAleatoria, columnaAleatoria);
            
            if (casillaSpawn != nullptr) {
                // Elegir tipo de enemigo aleatoriamente (50% cada uno)
                int tipoEnemigo = std::rand() % 2;
                
                if (tipoEnemigo == 0) {
                    // Crear Enemigo_Profesor (lento)
                    enemigos.push_back(std::make_unique<Enemigo_Profesor>(*casillaSpawn, "./assets/images/profesor.png"));
                } else {
                    // Crear Enemigo_Integral (rápido, zigzag)
                    enemigos.push_back(std::make_unique<Enemigo_Integral>(*casillaSpawn, "./assets/images/intEnemy.png"));
                }
            }
            
            // Reiniciar reloj con nuevo tiempo aleatorio (entre 2-6 segundos)
            tiempoProximoSpawn = 2.0f + static_cast<float>(std::rand() % 4);
            relojSpawn.restart();
        }
        
        // Actualizar todos los enemigos (IA)
        for (auto& enemigo : enemigos) {
            enemigo->actualizar(tablero);
        }
        
        gameWindow.clear();
        background.Dibujar(gameWindow);  // Dibujar background primero
        tablero.Dibujar(gameWindow);
        
        // Dibujar todos los enemigos
        for (auto& enemigo : enemigos) {
            enemigo->Dibujar(gameWindow);
        }
        
        jugador.Dibujar(gameWindow);
        gameWindow.display();
        //Arthur estuvo aqui
        // ok
        
    }

    return 0;
}

