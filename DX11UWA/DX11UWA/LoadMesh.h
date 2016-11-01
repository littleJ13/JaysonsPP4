#pragma once
#include "Content\ShaderStructures.h"
#include "pch.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace DirectX;
using namespace DX11UWA;

struct Mesh
{
	const char * TextureName;
	std::vector<VertexPositionUVNormal> VPUVN;
	std::vector<unsigned short> VertexIndices;
};

void LoadThatMesh(const char *path, Mesh & model)
{
	std::ifstream Input;
	Input.open(path, std::ios_base::binary);

	if (Input.is_open())
	{
		unsigned int NameLength = 0;
		Input.read((char*)&NameLength, sizeof(NameLength));
		char* MeshName = new char[NameLength];
		Input.read(MeshName, NameLength);

		unsigned int TextureNameLength = 0;
		Input.read((char*)&TextureNameLength, sizeof(TextureNameLength));
		for (unsigned int i = 0; i < TextureNameLength; ++i)
		{
			unsigned int TextureNameLength2 = 0;
			Input.read((char*)&TextureNameLength2, sizeof(TextureNameLength2));
			char* TextureName = new char[TextureNameLength2];
			Input.read(TextureName, TextureNameLength2);
			std::string Name = TextureName;
			unsigned int pos = Name.find_last_of("/\\");
			Name = Name.substr(pos + 1);
			model.TextureName = Name.c_str();
		}

		unsigned int numVerts = 0;
		Input.read((char*)&numVerts, sizeof(numVerts));
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			VertexPositionUVNormal Vertex;
			Input.read((char *)&Vertex.pos.x, 4);
			Input.read((char *)&Vertex.pos.y, 4);
			Input.read((char *)&Vertex.pos.z, 4);
			Vertex.pos.w = 1;

			Input.read((char *)&Vertex.normal.x, 4);
			Input.read((char *)&Vertex.normal.y, 4);
			Input.read((char *)&Vertex.normal.z, 4);
			Vertex.normal.w = 1;

			Input.read((char *)&Vertex.uv.x, 4);
			Input.read((char *)&Vertex.uv.y, 4);

			//For DirectX
			Vertex.uv.z = 1;
			Vertex.uv.w = 1;
			Vertex.uv.y = 1 - Vertex.uv.y;

			model.VPUVN.push_back(Vertex);
		}

		unsigned int Triangles = 0;
		Input.read((char*)&Triangles, sizeof(Triangles));
		for (unsigned int i = 0; i < Triangles * 3; ++i)
		{
			unsigned int temp = 0;
			Input.read((char*)&temp, sizeof(temp));
			model.VertexIndices.push_back(temp);
		}
		delete MeshName;
	}
}


