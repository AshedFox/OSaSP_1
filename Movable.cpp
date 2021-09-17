#include "Movable.h"

void Movable::SetMove(MoveDirection moveDirection)
{
	this->moveDirection = (MoveDirection)(this->moveDirection | moveDirection);
}

bool Movable::CheckIfMove(MoveDirection moveDirection) 
{
	return this->moveDirection & moveDirection;
}

void Movable::UnsetMove(MoveDirection moveDirection)
{
	this->moveDirection = (MoveDirection)(this->moveDirection & ~moveDirection);
}

void Movable::ResetMoveAll()
{
	this->moveDirection = MoveDirection::None;
}
