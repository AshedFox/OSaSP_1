#pragma once

enum MoveDirection {
	None = 0,
	Left = 1,
	Right = 2,
	Up = 4,
	Down = 8
};

class Movable
{
	public:
	MoveDirection moveDirection = MoveDirection::None;
	float xSpeed = 0.0f;
	float ySpeed = 0.0f;
	int xDirection = 1;
	int yDirection = 1;
	int posX = 0;
	int posY = 0;
	int width = 30;
	int height = 20;
	bool isMouseMoving = false;

	void SetMove(MoveDirection moveDirection);
	bool CheckIfMove(MoveDirection moveDirection);
	void UnsetMove(MoveDirection moveDirection);
	void ResetMoveAll();
};

