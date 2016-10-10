#ifndef _TEXT_H_
#define _TEXT_H_

#include "Font.h"
#include "FontShader.h"

class Text
{
private:
	struct Sentence
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	Text();
	Text(const Text& other);
	~Text();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd, int screenWidth, int screenHeight, char* fontDataFile, WCHAR* fontDDSFile, int textMaxLength, XMFLOAT4X4 baseViewMatrix);
	bool Render(ID3D11DeviceContext* context, XMFLOAT4X4 world, XMFLOAT4X4 ortho);
	void Shutdown();
	bool SetMousePosition(ID3D11DeviceContext* context, int mouseX, int mouseY);

private:
	bool InitSentence(ID3D11Device* device, Sentence** sentence, int maxLength);
	bool UpdateSentence(ID3D11DeviceContext* context, Sentence* sentence, char* text, int posX, int posY, float red, float green, float blue);
	void ReleaseSentence(Sentence** sentence);
	bool RenderSentence(ID3D11DeviceContext* context, Sentence* sentence, XMFLOAT4X4 world, XMFLOAT4X4 ortho);

private:

	Font* mFont;
	FontShader* mFontShader;
	int mScreenWidth, mScreenHeight;
	XMFLOAT4X4 mBaseViewMatrix;
	Sentence *mSentence1, *mSentence2;
};

#endif