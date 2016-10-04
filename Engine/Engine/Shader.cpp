#include "Shader.h"

Shader::Shader()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

Shader::Shader(const Shader& other)
{
}

Shader::~Shader()
{
}

bool Shader::Init(HWND hwnd, ID3D11Device* device)
{
	Check(InitShader(hwnd, device, L"Shaders/color.vs", L"Shaders/color.ps"));

	return true;
}

bool Shader::Render(ID3D11DeviceContext* context, int indexCount, XMFLOAT4X4 world, 
	XMFLOAT4X4 view, XMFLOAT4X4 proj)
{
	Check(SetShaderParameters(context, world, view, proj));

	RenderShader(context, indexCount);

	return true;
}

void Shader::Shutdown()
{
	ShutdownShader();
}

bool Shader::InitShader(HWND hwnd, ID3D11Device* device, WCHAR* vsPath, WCHAR* psPath)
{
	HRESULT result;

	ID3D10Blob* errorMsg = nullptr;
	ID3D10Blob* vsBuffer = nullptr;

	result = D3DCompileFromFile(vsPath, NULL, NULL, "VS", "vs_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBuffer, &errorMsg);
	
	if (FAILED(result))
	{
		if (errorMsg)
			OutputShaderErrorMessage(hwnd, errorMsg, vsPath);
		else
			MessageBox(hwnd, vsPath, L"Missing Shader File.", MB_OK);

		return false;
	}
		
	ID3D10Blob* psBuffer = nullptr;
	result = D3DCompileFromFile(psPath, NULL, NULL, "PS", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBuffer, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
			OutputShaderErrorMessage(hwnd, errorMsg, psPath);
		else
			MessageBox(hwnd, psPath, L"Missing Shader File.", MB_OK);

		return false;
	}

	HR(device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &mVertexShader));
	HR(device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &mPixelShader));

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	HR(device->CreateInputLayout(polygonLayout, numElements, vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), &mLayout));

	ReleaseCOM(vsBuffer);
	ReleaseCOM(psBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	HR(device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer));

	return true;
}

bool Shader::SetShaderParameters(ID3D11DeviceContext* context, XMFLOAT4X4 world,
	XMFLOAT4X4 view, XMFLOAT4X4 proj)
{
	XMMATRIX w = XMMatrixTranspose(XMLoadFloat4x4(&world));
	XMMATRIX v = XMMatrixTranspose(XMLoadFloat4x4(&view));
	XMMATRIX p = XMMatrixTranspose(XMLoadFloat4x4(&proj));

	D3D11_MAPPED_SUBRESOURCE mappedRes;
	MatrixBuffer* dataPtr;

	HR(context->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));

	dataPtr = (MatrixBuffer*)mappedRes.pData;

	XMStoreFloat4x4(&(dataPtr->worldMatrix), w);
	XMStoreFloat4x4(&(dataPtr->viewMatrix), v);
	XMStoreFloat4x4(&(dataPtr->projMatrix), p);

	context->Unmap(mMatrixBuffer, 0);
	context->VSSetConstantBuffers(0, 1, &mMatrixBuffer);

	return true;
}

void Shader::RenderShader(ID3D11DeviceContext* context, int indexCount)
{
	context->IASetInputLayout(mLayout);

	context->VSSetShader(mVertexShader, NULL, 0);
	context->PSSetShader(mPixelShader, NULL, 0);

	context->DrawIndexed(indexCount, 0, 0);
}

void Shader::ShutdownShader()
{
	ReleaseCOM(mMatrixBuffer);
	ReleaseCOM(mLayout);
	ReleaseCOM(mPixelShader);
	ReleaseCOM(mVertexShader);
}

void Shader::OutputShaderErrorMessage(HWND hwnd, ID3D10Blob* error, WCHAR* path)
{
	char* errorInfo = static_cast<char*>(error->GetBufferPointer());
	unsigned long size = error->GetBufferSize();

	std::ofstream fout("shader_compile_log.txt", std::ios::out);
	for (unsigned long i = 0; i < size; ++i)
		fout << errorInfo[i];
	fout.close();

	ReleaseCOM(error);
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", path, MB_OK);
}