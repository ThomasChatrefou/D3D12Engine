#pragma once


class InputManager
{
public:
	InputManager();

	void Update();

	bool IsKeyDown(int key) const;
	bool IsKeyUp(int key) const;
private: 
	bool keys[256];
};
#include <Windows.h>
InputManager::InputManager()
{
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}
void InputManager::Update()
{
	for (int i = 0; i < 256; i++)
	{
		keys[i] = GetAsyncKeyState(i) & 0x8000 ? true : false;
	}
}
bool InputManager::IsKeyDown(int key) const
{
	return keys[key];
}

bool InputManager::IsKeyUp(int key) const
{
	return !keys[key];
}