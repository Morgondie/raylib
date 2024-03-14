#pragma once
#include "GameObject.hpp"

class Jugador : public GameObject
{
private:


public:

	/*Jugador(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		xa = x1;
		xb = x2;
		xc = x3;
		ya = y1;
		yb = y2;
		yc = y3;
	}*/

	void Draw(int x1, int y1, int x2, int y2, int x3, int y3)
	{

		if (linePoints.empty())
			drawTriangle(x1, y1, x2, y2, x3, y3, WHITE);
		else
			drawPixelLinePoints(WHITE);
	}

	void Update()
	{

		if (IsKeyDown(KEY_RIGHT)) {
			MoveMatrix(2, 0); // Move the triangle 2 units to the right
			// Update the coordinates of the triangle after moving
		}
		if (IsKeyDown(KEY_LEFT)) {
			MoveMatrix(-2, 0); // Move the triangle 2 units to the left
			// Update the coordinates of the triangle after moving
		}
		if (IsKeyDown(KEY_UP)) {
			MoveMatrix(0, -2); // Move the triangle 2 units up
			// Update the coordinates of the triangle after moving}
		}
		if (IsKeyDown(KEY_DOWN)) {
			MoveMatrix(0, 2); // Move the triangle 2 units down
		}

		if (IsKeyDown(KEY_RIGHT_SHIFT)) {
			RotateMatrix(1);
		}
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			RotateMatrix(-1);
		}

	}

};