#pragma once
#include <vector>

class Keyboard
{
public:
	bool IsKeyDown(char key);
	void SetKey(char key, bool isDown);
	void Init();
private:
	std::vector<bool> _keyStatus;
};

