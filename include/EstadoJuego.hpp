#pragma once
#include <SFML/Graphics.hpp>
#include <Pantalla.hpp>
#include <string>

class GestorJuego {
private:
    int vidas;
    int score;
    int scoreMaximo;
    const int PUNTOS_POR_CASILLA = 1000;
    sf::Font fuente;
    bool fuenteCargada;

public:
    GestorJuego() : vidas(3), score(0), scoreMaximo(0), fuenteCargada(false) {
        fuenteCargada = fuente.loadFromFile("./assets/fonts/Supersonic Rocketship.ttf");
        if (!fuenteCargada) {
            fuente.loadFromFile("./assets/fonts/arial.ttf");
        }
    }

    void reiniciar(int numFilas) {
        vidas = 3;
        score = 0;
        
        // Calcular score máximo (número total de casillas * 1000)
        int totalCasillas = 0;
        for (int i = 0; i < numFilas; i++) {
            totalCasillas += (i + 1); // Fila 0 tiene 1 casilla, fila 1 tiene 2, etc.
        }
        scoreMaximo = totalCasillas * PUNTOS_POR_CASILLA;
    }

    void perderVida() {
        if (vidas > 0) {
            vidas--;
        }
    }

    void agregarPuntos() {
        score += PUNTOS_POR_CASILLA;
        if (score > scoreMaximo) {
            score = scoreMaximo;
        }
    }

    void restarPuntos() {
        score -= PUNTOS_POR_CASILLA;
        if (score < 0) {
            score = 0;
        }
    }

    int getVidas() const { return vidas; }
    int getScore() const { return score; }
    int getScoreMaximo() const { return scoreMaximo; }
    bool tieneVidas() const { return vidas > 0; }
    bool alcanzoScoreMaximo() const { return score >= scoreMaximo - 2000; }

    void dibujarHUD(Pantalla& window) {
        if (!fuenteCargada) return;

        // Texto de vidas
        sf::Text textoVidas;
        textoVidas.setFont(fuente);
        textoVidas.setString("Vidas: " + std::to_string(vidas));
        textoVidas.setCharacterSize(30);
        textoVidas.setFillColor(sf::Color::White);
        textoVidas.setPosition(10.f, 10.f);
        window.draw(textoVidas);

        // Texto de score
        sf::Text textoScore;
        textoScore.setFont(fuente);
        textoScore.setString("Score: " + std::to_string(score));
        textoScore.setCharacterSize(30);
        textoScore.setFillColor(sf::Color::Yellow);
        textoScore.setPosition(10.f, 50.f);
        window.draw(textoScore);
    }
};
