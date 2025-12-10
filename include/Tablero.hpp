#pragma once
#include <SFML/Graphics.hpp>
#include <Casilla.hpp>
#include <Pantalla.hpp>
#include <vector>
using namespace std;


class Tablero
{
private:
    int altura;
    
    void piramide(int h)
    {
        altura = h;
        casillas.resize(altura);
        
        for (int r = 0; r < altura; r++)
        {
            casillas[r].reserve(r + 1);  // Reservar espacio para evitar reubicaciones
            for (int c = 0; c <= r; c++)
            {
                float x = (c * 64.f) - (r * 64.f / 2.f);
                float y = (r * 45.f);
                Casilla casilla(450.f + x, 80.f + y, r, c);
                casillas[r].push_back(casilla);
            }
        }
    }

public:
    vector<vector<Casilla>> casillas;  // casillas[fila][columna]

    Casilla* getCasilla(int fila, int columna) {
        if (fila >= 0 && fila < altura && columna >= 0 && columna <= fila) {
            return &casillas[fila][columna];
        }
        return nullptr;
    }
    
    Tablero(int altura) {
        piramide(altura);
    }
    ~Tablero() {}
    
    int getAltura() const { return altura; }
    
    void Dibujar(Pantalla &window)
    {
        for (auto &fila : casillas)
        {
            for (auto &casilla : fila)
            {
                casilla.Dibujar(window);
            }
        }
    }

};