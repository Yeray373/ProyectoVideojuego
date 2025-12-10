#include <SFML/Graphics.hpp>
#include <Pantalla.hpp>
#include <Jugador.hpp>
#include <Enemigo.hpp>
#include <Enemigo_Profesor.hpp>
#include <Enemigo_Integral.hpp>
#include <Casilla.hpp>
#include <Background.hpp>
#include <Menu.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <Tablero.hpp>

enum class EstadoJuego {
    MENU,
    JUGANDO,
    PAUSADO
};

int main() {
    // Inicializar generador de números aleatorios
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    Pantalla gameWindow;
    
    if (!gameWindow.isOpen()) {
        return -1;
    }
    
    Menu menu;
    
    EstadoJuego estadoActual = EstadoJuego::MENU;
    
    // Variables del juego (se inicializarán cuando se seleccione JUGAR)
    std::unique_ptr<Background> background;
    std::unique_ptr<Tablero> tablero;
    std::unique_ptr<Jugador> jugador;
    std::vector<std::unique_ptr<Enemigo>> enemigos;
    sf::Clock relojSpawn;
    float tiempoProximoSpawn = 2.0f;
    int maxEnemigos = 2;

    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            
            // Manejar eventos según el estado
            if (estadoActual == EstadoJuego::MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (menu.estaAjustandoVolumen()) {
                        if (event.key.code == sf::Keyboard::Left) {
                            menu.disminuirVolumen();
                        } else if (event.key.code == sf::Keyboard::Right) {
                            menu.aumentarVolumen();
                        } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape) {
                            menu.seleccionar(); // Salir del ajuste de volumen
                        }
                    } else {
                        if (event.key.code == sf::Keyboard::Up) {
                            menu.moverArriba();
                        } else if (event.key.code == sf::Keyboard::Down) {
                            menu.moverAbajo();
                        } else if (event.key.code == sf::Keyboard::Enter) {
                            OpcionMenu opcion = menu.seleccionar();
                            if (opcion == OpcionMenu::JUGAR) {
                                // Inicializar el juego
                                background = std::make_unique<Background>("./assets/images/Menu.png");
                                tablero = std::make_unique<Tablero>(7);
                                jugador = std::make_unique<Jugador>(tablero->casillas[0][0], "./assets/images/cetiano.png");
                                enemigos.clear();
                                relojSpawn.restart();
                                tiempoProximoSpawn = 2.0f + static_cast<float>(std::rand() % 4);
                                estadoActual = EstadoJuego::JUGANDO;
                            } else if (opcion == OpcionMenu::SALIR) {
                                gameWindow.close();
                            }
                        }
                    }
                }
            } else if (estadoActual == EstadoJuego::JUGANDO) {
                // Eventos del juego
                if (event.type == sf::Event::KeyPressed) {
                    int filaActual = jugador->getCasillaActual().getFila();
                    int columnaActual = jugador->getCasillaActual().getColumna();
                if (!jugador->getEstaVivo()) {
                    // Si el jugador está muerto, presionar cualquier tecla lo respawnea
                    if (event.key.code == sf::Keyboard::Space) {
                        jugador->respawn();
                    }
                } else {
                    // Movimiento diagonal abajo-derecha (Down + Right)
                    if (event.key.code == sf::Keyboard::Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        int filaDestino = filaActual + 1;
                        int colDestino = columnaActual + 1;
                        Casilla* nuevaCasilla = tablero->getCasilla(filaDestino, colDestino);
                        jugador->intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                    // Movimiento diagonal abajo-izquierda (Down + Left)
                    else if (event.key.code == sf::Keyboard::Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        int filaDestino = filaActual + 1;
                        int colDestino = columnaActual;
                        Casilla* nuevaCasilla = tablero->getCasilla(filaDestino, colDestino);
                        jugador->intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                    // Movimiento diagonal arriba-derecha (Up + Right)
                    else if (event.key.code == sf::Keyboard::Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        int filaDestino = filaActual - 1;
                        int colDestino = columnaActual;
                        Casilla* nuevaCasilla = tablero->getCasilla(filaDestino, colDestino);
                        jugador->intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                    // Movimiento diagonal arriba-izquierda (Up + Left)
                    else if (event.key.code == sf::Keyboard::Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        int filaDestino = filaActual - 1;
                        int colDestino = columnaActual - 1;
                        Casilla* nuevaCasilla = tablero->getCasilla(filaDestino, colDestino);
                        jugador->intentarMover(nuevaCasilla, filaDestino, colDestino);
                    }
                }
                } // Cierre del if KeyPressed del juego
            } // Cierre del else if JUGANDO
        } // Cierre del while pollEvent
        
        // Lógica y renderizado según el estado
        if (estadoActual == EstadoJuego::MENU) {
            gameWindow.clear(sf::Color(10, 10, 20));
            menu.dibujar(gameWindow);
            gameWindow.display();
        } else if (estadoActual == EstadoJuego::JUGANDO) {
            // Sistema de spawn de enemigos
            if (relojSpawn.getElapsedTime().asSeconds() >= tiempoProximoSpawn && enemigos.size() < maxEnemigos) {
                // Generar fila y columna aleatoria
                int filaAleatoria = std::rand() % tablero->getAltura();
                int columnaAleatoria = std::rand() % (filaAleatoria + 1);
                
                // Obtener la casilla
                Casilla* casillaSpawn = tablero->getCasilla(filaAleatoria, columnaAleatoria);
                
                if (casillaSpawn != nullptr) {
                    // Elegir tipo de enemigo aleatoriamente (50% cada uno)
                    int tipoEnemigo = std::rand() % 2;
                    
                    if (tipoEnemigo == 0) {
                        enemigos.push_back(std::make_unique<Enemigo_Profesor>(*casillaSpawn, "./assets/images/profesor.png"));
                    } else {
                        enemigos.push_back(std::make_unique<Enemigo_Integral>(*casillaSpawn, "./assets/images/intEnemy.png"));
                    }
                }
                
                tiempoProximoSpawn = 2.0f + static_cast<float>(std::rand() % 4);
                relojSpawn.restart();
            }
            
            // Actualizar todos los enemigos (IA)
            for (auto& enemigo : enemigos) {
                enemigo->actualizar(*tablero);
            }
            
            // Eliminar enemigos que hayan caído fuera de la pantalla
            enemigos.erase(
                std::remove_if(enemigos.begin(), enemigos.end(),
                    [](const std::unique_ptr<Enemigo>& enemigo) {
                        return enemigo->debeSerEliminado();
                    }),
                enemigos.end()
            );
            
            gameWindow.clear(sf::Color(0, 100, 100));
            background->Dibujar(gameWindow);
            tablero->Dibujar(gameWindow);
            
            for (auto& enemigo : enemigos) {
                enemigo->Dibujar(gameWindow);
            }
            
            jugador->Dibujar(gameWindow);
            gameWindow.display();
        }
    } // Cierre del while(gameWindow.isOpen())
    
    return 0;   
}




