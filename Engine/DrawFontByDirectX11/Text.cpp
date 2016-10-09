#include "Text.h"

Text::Text()
{
	mFont = nullptr;
	mFontShader = nullptr;

	mSentence1 = nullptr;
	mSentence2 = nullptr;
}

Text::Text(const Text& other)
{
}

Text::~Text()
{
}

bool Text::Init(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd, int screenWidth, int screenHeight, XMFLOAT4X4 baseViewMatrix)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mBaseViewMatrix = baseViewMatrix;

	mFont = new Font;
	Check(mFont->Init(device, "Data/fontdata.txt", L"Data/font.dds"));

	mFontShader = new FontShader;
	Check(mFontShader->Init(hwnd, device));

	Check(InitSentence(device, &mSentence1, 16));
	//Check(UpdateSentence(context, mSentence1, "I'm a mouse.", 100, 100, 1.0f, 0.0f, 0.0f));
	Check(InitSentence(device, &mSentence2, 16));
	//Check(UpdateSentence(context, mSentence2, "Goodbye", 100, 200, 0.0f, 0.0f, 0.0f));

	return true;
}

bool Text::Render(ID3D11DeviceContext* context, XMFLOAT4X4 world, XMFLOAT4X4 ortho)
{
	Check(RenderSentence(context, mSentence1, world, ortho));
	Check(RenderSentence(context, mSentence2, world, ortho));

	return true;
}

void Text::Shutdown()
{
	ReleaseSentence(&mSentence1);
	ReleaseSentence(&mSentence2);

	if (mFontShader)
	{
		mFontShader->Shutdown();
		SafeDelete(mFontShader);
	}

	if (mFont)
	{
		mFont->Shutdown();
		SafeDelete(mFont);
	}
}

bool Text::InitSentence(ID3D11Device* device, Sentence** sentence, int maxLength)
{
	*sentence = new Sentence;
	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;

	Vertex* vertices = new Vertex[(*sentence)->vertexCount];
	unsigned long* indices = new unsigned long[(*sentence)->indexCount];

	memset(vertices, 0, sizeof(Vertex) * (*sentence)->vertexCount);
	for (int i = 0; i < (*sentence)->indexCount; ++i)
		indices[i] = i;

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.ByteWidth = sizeof(Vertex) * (*sentence)->vertexCount;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = vertices;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	HR(device->CreateBuffer(&vbDesc, &vData, &(*sentence)->vertexBuffer));

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	HR(device->CreateBuffer(&ibDesc, &iData, &(*sentence)->indexBuffer));

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool Text::UpdateSentence(ID3D11DeviceContext* context, Sentence* sentence, char* text, int posX, int posY, float red, float green, float blue)
{
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	int numLetters = static_cast<int>(strlen(text));
	if (numLetters > sentence->maxLength)
		return false;

	Vertex* vertices = new Vertex[sentence->vertexCount];
	memset(vertices, 0, sizeof(Vertex) * sentence->vertexCount);

	float drawX = static_cast<float>(posX - (mScreenWidth>>1));
	float drawY = static_cast<float>((mScreenHeight>>1) - posY);

	mFont->BuildVertexArray((void*)vertices, text, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE mappedRes;
	HR(context->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));

	Vertex* verticesPtr = (Vertex*)mappedRes.pData;
	memcpy(verticesPtr, (void*)vertices, sizeof(Vertex) * sentence->vertexCount);
	
	context->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

bool Text::RenderSentence(ID3D11DeviceContext* context, Sentence* sentence, XMFLOAT4X4 world, XMFLOAT4X4 ortho)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMFLOAT4 pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);
	Check(mFontShader->Render(context, sentence->indexCount, world, mBaseViewMatrix, ortho, pixelColor, mFont->GetTexture()));

	return true;
}

void Text::ReleaseSentence(Sentence** sentence)
{
	if (*sentence)
	{
		ReleaseCOM((*sentence)->vertexBuffer);
		ReleaseCOM((*sentence)->indexBuffer);
		SafeDelete(*sentence);
	}
}

bool Text::SetMousePosition(ID3D11DeviceContext* context, int mouseX, int mouseY)
{
	char tempString[16];
	char mouseString[16];
	bool result;


	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(context, mSentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(context, mSentence2, mouseString, 20, 40, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}