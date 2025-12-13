#pragma once

// clasificacion de estados de animación
enum class AnimacionEstado {
    PARADO,    // Parado
    SALTAR,    // Saltando
    CAER,      // Cayendo
    MORIR,      // Muriendo (para enemigos)
    DESAPARECER 
};

// Clase para definir y manejar animación
class Animacion {
private:
    int filaSprite;
    int columnaInicio;
    int numFrames;
    int anchoFrame;
    int altoFrame;
    float frameTime;
    bool loop;
    int frameActual;

public:
    
    Animacion(int fila = 0, int columna = 0, int frames = 1, int ancho = 16, 
              int alto = 16, float tiempo = 0.1f, bool loopear = true)
        : filaSprite(fila), columnaInicio(columna), numFrames(frames),
          anchoFrame(ancho), altoFrame(alto), frameTime(tiempo), 
          loop(loopear), frameActual(0) {}
    
    
    int getFilaSprite() const { return filaSprite; }
    int getColumnaInicio() const { return columnaInicio; }
    int getNumFrames() const { return numFrames; }
    int getAnchoFrame() const { return anchoFrame; }
    int getAltoFrame() const { return altoFrame; }
    float getFrameTime() const { return frameTime; }
    bool esLoop() const { return loop; }
    int getFrameActual() const { return frameActual; }
    
    
    void reiniciar() { frameActual = 0; }
    
    void avanzarFrame() {
        frameActual++;
        if (frameActual >= numFrames) {
            if (loop) {
                frameActual = 0;
            } else {
                frameActual = numFrames ;
            }
        }
    }
    
    bool haTerminado() const {
        return !loop && frameActual >= numFrames ;
    }
};
