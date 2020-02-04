#include "Keyboard.h"

bool Keyboard::IsKeyDown(char key)
{
	return _keyStatus[(int)key];
}

void Keyboard::SetKey(char key, bool isDown)
{
	_keyStatus[(int)key] = isDown;
}

void Keyboard::Init()
{
	_keyStatus.resize(255);
}
