#include "Input.h"

Input::Input()
{
}

Input::Input(const Input& other)
{
}

Input::~Input()
{
}

void Input::Init()
{
	for(auto& key : mKeys)
		key = false;
}

void Input::KeyDown(unsigned int key)
{
	mKeys[key] = true;
}

void Input::KeyUp(unsigned int key)
{
	mKeys[key] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	return mKeys[key];
}