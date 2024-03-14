#pragma once
#include "Shape.hpp"

using namespace std;

struct Points
{
    float x, y;
    int one = 1;
};

struct AABB {
    float minX, minY;
    float maxX, maxY;

    AABB() : minX(0), minY(0), maxX(0), maxY(0) {}
    AABB(float minX, float minY, float maxX, float maxY) : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}


    bool overlaps(const AABB& other) const {
        return maxX >= other.minX && minX <= other.maxX &&
            maxY >= other.minY && minY <= other.maxY;
    }
};

class GameObject : public Shapes
{

public:

    enum Type
    {
        Jugador = 1, Asteroides = 2, Balas = 3
    };

    bool verification = false;

    Type objectType;
    vector<Points> transformedPoints;
    mutable AABB cachedAABB; //un AABB que se guardara hasta que se detecten cambios en la posicion del objeto
    mutable bool needsAABBUpdate = true; //booleano para saber si se necesita cambiar el AABB
    //los dos mutables para que se puedan cambiar en las funciones constantes.

    // Actualiza MoveMatrix para aplicar traslación.
    void MoveMatrix(int distanceX, int distanceY) {
        vector<Points> newPoints; // Contenedor para los nuevos puntos trasladados.
        vector<float> newLinePoints; //nuevo vector float para remplazar

        // Matriz de traslación.
        float translationMatrix[3][3] = {
            {1, 0, distanceX},
            {0, 1, distanceY},
            {0, 0, 1}
        };

        // Aplica la matriz de traslación a cada punto.
        for (size_t i = 0; i < linePoints.size(); i += 2) {
            Points originalPoint = { linePoints[i], linePoints[i + 1], 1 };
            Points transformedPoint;

            transformedPoint.x = (originalPoint.x * translationMatrix[0][0]) + (originalPoint.y * translationMatrix[0][1]) + (originalPoint.one * translationMatrix[0][2]);
            transformedPoint.y = (originalPoint.x * translationMatrix[1][0]) + (originalPoint.y * translationMatrix[1][1]) + (originalPoint.one * translationMatrix[1][2]);
            // uno es uno pero se multiplica para que no haya errores
            transformedPoint.one = (originalPoint.x * translationMatrix[2][0]) + (originalPoint.y * translationMatrix[2][1]) + (originalPoint.one * translationMatrix[2][2]);

            newPoints.push_back(transformedPoint);
        }
        // Convierte los puntos transformados de nuevo a vector<float> para el dibujo.
        for (const Points& p : newPoints) {
            newLinePoints.push_back(p.x);
            newLinePoints.push_back(p.y);
        }

        // Actualiza linePoints con los nuevos puntos trasladados.
        needsAABBUpdate = true;
        linePoints = newLinePoints;
    }

    void RotateMatrix(float angleDegrees) {
        vector<Points> newPoints;
        vector<float> newLinePoints;

        // Calcula el centro de la figura.
        Points center = CalculateCenter();

        float angleRadians = angleDegrees * (PI / 180.0f);
        float cosTheta = cos(angleRadians);
        float sinTheta = sin(angleRadians);

        // Recorre todos los puntos y aplica la rotación.
        for (size_t i = 0; i < linePoints.size(); i += 2) {
            float x = linePoints[i] - center.x; // Traslada el punto al origen (centro).
            float y = linePoints[i + 1] - center.y;

            // Aplica rotación.
            float newX = x * cosTheta - y * sinTheta;
            float newY = x * sinTheta + y * cosTheta;

            // Traslada el punto de vuelta a su posición original.
            newX += center.x;
            newY += center.y;

            // Guarda los puntos rotados.
            newLinePoints.push_back(newX);
            newLinePoints.push_back(newY);
        }


        needsAABBUpdate = true;
        linePoints = newLinePoints;
    }


    Points CalculateCenter() {
        float sumX = 0, sumY = 0;
        int pointsCount = linePoints.size() / 2;
        for (size_t i = 0; i < linePoints.size(); i += 2) {
            sumX += linePoints[i];
            sumY += linePoints[i + 1];
        }
        return { sumX / pointsCount, sumY / pointsCount, 1 };
    }



    void ScaleMatrix(float scaleX, float scaleY) {
        vector<float> newLinePoints; // Nuevo vector float para reemplazar linePoints.

        // Calcula el centro de la figura.
        Points center = CalculateCenter();

        // Recorre todos los puntos y aplica el escalado.
        for (size_t i = 0; i < linePoints.size(); i += 2) {
            float x = linePoints[i] - center.x; // Traslada el punto al origen (centro).
            float y = linePoints[i + 1] - center.y;

            // Aplica escalado.
            float newX = x * scaleX;
            float newY = y * scaleY;

            // Traslada el punto de vuelta a su posición original.
            newX += center.x;
            newY += center.y;

            // Guarda los puntos escalados.
            newLinePoints.push_back(newX);
            newLinePoints.push_back(newY);
        }

        // Actualiza linePoints con los puntos escalados.
        linePoints = newLinePoints;
    }

    // Agrega en la clase GameObject
    AABB calculateAABB() const {
        if (!needsAABBUpdate) {

            return cachedAABB;
        }

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();

        for (size_t i = 0; i < linePoints.size(); i += 2) {
            float x = linePoints[i];
            float y = linePoints[i + 1];
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
        }

        cachedAABB = AABB(minX, minY, maxX, maxY);
        needsAABBUpdate = false;

        return cachedAABB;
    }

    void ClearLinePoints() {
        linePoints.clear();
    }

    bool Collider(const GameObject& other) const {
        AABB box1 = this->calculateAABB();
        AABB box2 = other.calculateAABB();

        return box1.overlaps(box2);
    }
};