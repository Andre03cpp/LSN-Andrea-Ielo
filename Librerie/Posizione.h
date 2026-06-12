#ifndef __Posizione_h__
#define __Posizione_h__

#include <iostream>
#include <cmath>


class Posizione {
public:
    // Costruttori
    Posizione() : m_x(0), m_y(0), m_z(0) {} // Uso la lista di inizializzazione. Si chiama Member Initializer List
    Posizione(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}

    // Getter
    double getX() const { return m_x; }
    double getY() const { return m_y; }
    double getZ() const { return m_z; }

    // Setter corretti
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setZ(double z) { m_z = z; }

    // Utile per il Random Walk: distanza dall'origine al quadrato e non
    double getR2() const { return m_x*m_x + m_y*m_y + m_z*m_z; }
    double getR()  const { return std::sqrt(getR2()); }

    // Per muovere il punto senza dover fare get/set ogni volta
    void sposta(double dx, double dy, double dz) {
        m_x += dx; m_y += dy; m_z += dz;
    }

    void Print() const {
        std::cout << "Posizione: (" << m_x << ", " << m_y << ", " << m_z << ")" << std::endl;
    }

protected:
    double m_x, m_y, m_z;
};

#endif
