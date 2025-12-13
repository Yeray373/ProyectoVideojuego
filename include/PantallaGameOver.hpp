#pragma once
#include <SFML/Graphics.hpp>
#include <Pantalla.hpp>
#include <string>

class PantallaGameOver {
private:
    sf::Font fuente, fuenteCalificacion;
    sf::Text textoGameOver;
    sf::Text Calificacion;
    sf::Text textoInstrucciones;
    sf::RectangleShape fondo;
    bool esVictoria;
    int scoreActual;
    int scoreMaximo;
    
    // Imagen de fondo opcional
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    bool tieneImagenFondo;

public:
    PantallaGameOver() : tieneImagenFondo(false), esVictoria(false), scoreActual(0), scoreMaximo(26000) {
        // Cargar fuente
        if (!fuente.loadFromFile("./assets/fonts/Supersonic Rocketship.ttf")) {
            fuente.loadFromFile("C:/Windows/Fonts/Arial.ttf");
        }
        if (!fuenteCalificacion.loadFromFile("./assets/fonts/pixel-love.ttf")) {
            fuenteCalificacion.loadFromFile("C:/Windows/Fonts/Arial.ttf");
        }
        
        // Intentar cargar imagen de fondo
        if (texturaFondo.loadFromFile("./assets/images/GameOver.png")) {
            tieneImagenFondo = true;
            spriteFondo.setTexture(texturaFondo);
            
            // Escalar la imagen para que cubra toda la ventana
            sf::Vector2u tamañoTextura = texturaFondo.getSize();
            float escalaX = 900.f / tamañoTextura.x;
            float escalaY = 600.f / tamañoTextura.y;
            spriteFondo.setScale(escalaX, escalaY);
            spriteFondo.setPosition(0, 0);
        }
        
        // Configurar fondo de respaldo
        fondo.setSize(sf::Vector2f(900.f, 600.f));
        fondo.setFillColor(sf::Color(20, 20, 20));
        fondo.setPosition(0, 0);

        Calificacion.setFont(fuenteCalificacion);
        Calificacion.setCharacterSize(60);
        Calificacion.setFillColor(sf::Color::Red);
        Calificacion.setPosition(720.f, 110.f);
        Calificacion.setRotation(15.f);
        
        // Configurar texto de Game Over
        textoGameOver.setFont(fuente);
        textoGameOver.setCharacterSize(100);
        textoGameOver.setFillColor(sf::Color::Red);
        textoGameOver.setPosition(40.f, 25.f);
        
        // Configurar instrucciones
        textoInstrucciones.setFont(fuente);
        textoInstrucciones.setString("Presiona ENTER \npara volver al menu");
        textoInstrucciones.setCharacterSize(25);
        textoInstrucciones.setFillColor(sf::Color::White);
        textoInstrucciones.setPosition(40.f, 250.f);
    }
    
    void configurarVictoria(bool victoria, int score = 0, int scoreMax = 26000) {
        esVictoria = victoria;
        scoreActual = score;
        scoreMaximo = scoreMax;
        
        if (esVictoria) {
            textoGameOver.setString("VICTORIA!");
            textoGameOver.setFillColor(sf::Color::Green);
            Calificacion.setString("100");
            Calificacion.setFillColor(sf::Color::Red);
        } else {
            textoGameOver.setString("GAME \nOVER");
            
            // Calcular porcentaje: (scoreActual * 100) / scoreMaximo
            int porcentaje = (scoreMaximo > 0) ? (scoreActual * 100) / scoreMaximo : 0;
            Calificacion.setString(std::to_string(porcentaje));
            Calificacion.setFillColor(sf::Color::Red);
        }
    }
    
    void dibujar(Pantalla& window) {
        // Dibujar fondo
        if (tieneImagenFondo) {
            window.draw(spriteFondo);
        } else {
            window.draw(fondo);
        }
        
        // Dibujar textos
        window.draw(textoGameOver);
        window.draw(Calificacion);
        window.draw(textoInstrucciones);
    }
};
