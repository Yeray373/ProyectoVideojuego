#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Pantalla.hpp>
#include <vector>
#include <string>
#include <iostream>

enum class OpcionMenu {
    JUGAR,
    VOLUMEN,
    SALIR,
    NINGUNA
};

class Menu {
private:
    sf::Font fuente;
    sf::Text titulo;
    std::vector<sf::Text> opciones;
    int opcionSeleccionada;
    sf::RectangleShape fondo;
    
    // Imagen de fondo personalizada
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    bool tieneImagenFondo;
    
    // Para el control de volumen
    bool ajustandoVolumen;
    float volumenActual;
    sf::RectangleShape barraVolumen;
    sf::RectangleShape indicadorVolumen;
    sf::Text textoVolumen;

public:
    Menu() : opcionSeleccionada(0), ajustandoVolumen(false), volumenActual(50.f), tieneImagenFondo(false) {
        // Cargar fuente (probar múltiples rutas)
        bool fuenteCargada = false;
        
        if (fuente.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            fuenteCargada = true;
            std::cout << "Fuente cargada: arial.ttf" << std::endl;
        } else if (fuente.loadFromFile("C:/Windows/Fonts/Arial.ttf")) {
            fuenteCargada = true;
            std::cout << "Fuente cargada: Arial.ttf" << std::endl;
        } else if (fuente.loadFromFile("./assets/fonts/arial.ttf")) {
            fuenteCargada = true;
            std::cout << "Fuente cargada desde assets" << std::endl;
        } else {
            std::cout << "ERROR: No se pudo cargar ninguna fuente!" << std::endl;
        }
        
        // Intentar cargar imagen de fondo personalizada
        if (texturaFondo.loadFromFile("./assets/images/Background.png")) {
            tieneImagenFondo = true;
            spriteFondo.setTexture(texturaFondo);
            
            // Escalar la imagen para que cubra toda la ventana
            sf::Vector2u tamañoTextura = texturaFondo.getSize();
            float escalaX = 900.f / tamañoTextura.x;
            float escalaY = 600.f / tamañoTextura.y;
            spriteFondo.setScale(escalaX, escalaY);
            spriteFondo.setPosition(0, 0);
        }
        
        // Configurar fondo de respaldo (si no hay imagen)
        fondo.setSize(sf::Vector2f(900.f, 600.f));
        fondo.setFillColor(sf::Color(20, 20, 40, 250));
        fondo.setPosition(0, 0);
        
        // Configurar título
        titulo.setFont(fuente);
        titulo.setString("JUEGO DE PIRAMIDE");
        titulo.setCharacterSize(60);
        titulo.setFillColor(sf::Color::Yellow);
        titulo.setPosition(200.f, 80.f);
        
        // Configurar opciones del menú
        std::vector<std::string> textosOpciones = {"JUGAR", "VOLUMEN", "SALIR"};
        float posY = 250.f;
        
        for (size_t i = 0; i < textosOpciones.size(); i++) {
            sf::Text opcion;
            opcion.setFont(fuente);
            opcion.setString(textosOpciones[i]);
            opcion.setCharacterSize(40);
            opcion.setFillColor(sf::Color::White);
            opcion.setPosition(350.f, posY + (i * 80.f));
            opciones.push_back(opcion);
        }
        
        // Configurar barra de volumen
        barraVolumen.setSize(sf::Vector2f(300.f, 20.f));
        barraVolumen.setFillColor(sf::Color(100, 100, 100));
        barraVolumen.setPosition(300.f, 380.f);
        barraVolumen.setOutlineThickness(2.f);
        barraVolumen.setOutlineColor(sf::Color::White);
        
        indicadorVolumen.setSize(sf::Vector2f(150.f, 20.f));
        indicadorVolumen.setFillColor(sf::Color::Green);
        indicadorVolumen.setPosition(300.f, 380.f);
        
        textoVolumen.setFont(fuente);
        textoVolumen.setString("Volumen: 50%");
        textoVolumen.setCharacterSize(25);
        textoVolumen.setFillColor(sf::Color::White);
        textoVolumen.setPosition(350.f, 340.f);
        
        actualizarColorOpciones();
    }
    
    void moverArriba() {
        if (!ajustandoVolumen && opcionSeleccionada > 0) {
            opcionSeleccionada--;
            actualizarColorOpciones();
        }
    }
    
    void moverAbajo() {
        if (!ajustandoVolumen && opcionSeleccionada < static_cast<int>(opciones.size()) - 1) {
            opcionSeleccionada++;
            actualizarColorOpciones();
        }
    }
    
    void aumentarVolumen() {
        if (ajustandoVolumen && volumenActual < 100.f) {
            volumenActual += 5.f;
            if (volumenActual > 100.f) volumenActual = 100.f;
            actualizarVolumen();
        }
    }
    
    void disminuirVolumen() {
        if (ajustandoVolumen && volumenActual > 0.f) {
            volumenActual -= 5.f;
            if (volumenActual < 0.f) volumenActual = 0.f;
            actualizarVolumen();
        }
    }
    
    OpcionMenu seleccionar() {
        if (opcionSeleccionada == 0) {
            return OpcionMenu::JUGAR;
        } else if (opcionSeleccionada == 1) {
            ajustandoVolumen = !ajustandoVolumen;
            actualizarColorOpciones();
            return OpcionMenu::VOLUMEN;
        } else if (opcionSeleccionada == 2) {
            return OpcionMenu::SALIR;
        }
        return OpcionMenu::NINGUNA;
    }
    
    void dibujar(Pantalla& window) {
        // Dibujar imagen de fondo si existe, sino el fondo de color
        if (tieneImagenFondo) {
            window.draw(spriteFondo);
        } else {
            window.draw(fondo);
        }
        
        window.draw(titulo);
        
        for (auto& opcion : opciones) {
            window.draw(opcion);
        }
        
        // Mostrar controles de volumen si está ajustando
        if (ajustandoVolumen) {
            window.draw(textoVolumen);
            window.draw(barraVolumen);
            window.draw(indicadorVolumen);
        }
    }
    
    float getVolumen() const {
        return volumenActual;
    }
    
    bool estaAjustandoVolumen() const {
        return ajustandoVolumen;
    }
    
private:
    void actualizarColorOpciones() {
        for (size_t i = 0; i < opciones.size(); i++) {
            if (static_cast<int>(i) == opcionSeleccionada) {
                opciones[i].setFillColor(sf::Color::Yellow);
                opciones[i].setStyle(sf::Text::Bold);
            } else {
                opciones[i].setFillColor(sf::Color::White);
                opciones[i].setStyle(sf::Text::Regular);
            }
        }
        
        // Indicador especial cuando está ajustando volumen
        if (ajustandoVolumen && opcionSeleccionada == 1) {
            opciones[1].setString("VOLUMEN <->");
        } else {
            opciones[1].setString("VOLUMEN");
        }
    }
    
    void actualizarVolumen() {
        float ancho = (volumenActual / 100.f) * 300.f;
        indicadorVolumen.setSize(sf::Vector2f(ancho, 20.f));
        textoVolumen.setString("Volumen: " + std::to_string(static_cast<int>(volumenActual)) + "%");
    }
};
