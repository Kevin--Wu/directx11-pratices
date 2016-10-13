#ifndef _MODELLIST_H_
#define _MODELLIST_H_

class ModelList
{
public:
	ModelList();
	ModelList(const ModelList& other);
	~ModelList();

	bool Init();
	void Render();
	void Shutdown();

private:

};

#endif