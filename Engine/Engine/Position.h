#ifndef _POSITION_H_
#define _POSITION_H_

class Position
{
public:
	Position();
	Position(const Position& other);
	~Position();

	bool Init();
	void Render();
	void Shutdown();

private:

};

#endif