#ifndef _INPUT_H_
#define _INPUT_H_
class Input
{
public:
	Input();
	Input(const Input& other);
	~Input();

	void Init();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);

private:
	bool mKeys[256];
};
#endif