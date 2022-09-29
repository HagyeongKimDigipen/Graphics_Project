/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#include "MouseEvent.h"

MouseEvent::MouseEvent() :type(EventType::Invalid),	mouseX(0),mouseY(0)
{
	
}

MouseEvent::MouseEvent(EventType type, int x, int y) : type(type), mouseX(x), mouseY(y)
{
	
}

bool MouseEvent::IsValid() const
{
	return this->type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return this->type;
}

MousePoint MouseEvent::GetPos() const
{
	return{ this->mouseX,this->mouseY };
}

int MouseEvent::GetPosX() const
{
	return this->mouseX;
}

int MouseEvent::GetPosY() const
{
	return this->mouseY;
}
