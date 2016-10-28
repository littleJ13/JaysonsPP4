#pragma once
#include "App.h"
#include "Sample3DSceneRenderer.h"
#include <vector>

using namespace DirectX;

std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
std::vector<XMFLOAT3>tempVertices;
std::vector<XMFLOAT2>tempUVs;
std::vector<XMFLOAT3>tempNormals;

Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

void loadObj(const char* file, std::vector<XMFLOAT3> &outVertices, std::vector<XMFLOAT2> &outUVs, std::vector<XMFLOAT3> &outNormals)
{
	FILE * fileOpen = fopen(file, "r");
	if (fileOpen == NULL)
	{
		return false;
	}

	while (fileOpen != NULL)
	{
		char lineHeader[128];
		int res = fscanf(fileOpen, "%s", lineHeader);
		if (res == EOF)
			break;
	}

	if (strcmp(lineHeader, "v") == 0)
	{
		XMFLOAT3 Vertex;
		fscanf(fileOpen, "%f %f %f\n", &Vertex.x, &Vertex.y, &Vertex.z);
		tempVertices.push_back(Vertex);
	}

	else if (strcmp(lineHeader, "vt") == 0)
	{
		XMFLOAT2 UV;
		fscanf(fileOpen, "%f %f\n"), &UV.x, &UV.y);
		tempUVs.push_back(UV);
	}

	else if (strcmp(lineHeader, "vn") == 0)
	{
		XMFLOAT3 Normal;
		fscanf(fileOpen, "%f %f %f\n", &Normal.x, &Normal.y, &Normal.z);
		tempNormals.push_back(Normal);
	}

	else if (strcmp(lineHeader, "f") == 0)
	{
		std::string vertex1, vertex2, vertex3;
		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
		int matches = fscanf(fileOpen, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			&vertexIndex[1], &uvIndex[1], &normalIndex[1],
			&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

		if (matches != 9)
			return false;
		
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
		vertexIndices.push_back(vertexIndex[2]);
		uvIndices.push_back(uvIndex[0]);
		uvIndices.push_back(uvIndex[1]);
		uvIndices.push_back(uvIndex[2]);
		normalIndices.push_back(normalIndex[0]);
		normalIndices.push_back(normalIndex[1]);
		normalIndices.push_back(normalIndex[2]);
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		XMFLOAT3 vertex = tempVertices[vertexIndex - 1];
		outVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		unsigned int UVIndex = uvIndices[i];
		XMFLOAT2 uv = tempUVs[UVIndex - 1];
		outUVs.push_back(uv);
	}
	
	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		unsigned int NormalIndex = normalIndices[i];
		XMFLOAT3 normal = tempNormals[NormalIndex - 1];
		outNormals.push_back(vertex);
	}
}
