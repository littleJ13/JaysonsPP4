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
	std::vector<VertexPositionUVNormal> VertPosUVNorms;
	std::vector<unsigned short> VertexIndecies;

	void addPUVNVec(VertexPositionUVNormal puvn)
	{
		VertPosUVNorms.push_back(puvn);
	}
};

void LoadThatMesh(const char *path, Mesh & model)
{
	unsigned int meshNameLength = 0, numTextures = 0, textureNameLength = 0, numVerts = 0, numTri = 0, numIndex = 0;
	std::string name = "", name2 = "", name3 = "";
	char* textureName;

	std::ifstream stream;
	stream.open(path, std::ios_base::binary);

	if (stream.is_open())
	{
		stream.read((char*)&meshNameLength, sizeof(meshNameLength));
		char* meshName = new char[meshNameLength];
		stream.read(meshName, meshNameLength);


		stream.read((char*)&numTextures, sizeof(numTextures));
		for (unsigned int i = 0; i < numTextures; ++i)
		{
			int sub;

			stream.read((char*)&textureNameLength, sizeof(textureNameLength));
			textureName = new char[textureNameLength];
			stream.read(textureName, textureNameLength);

			if (name == "")
			{
				name = textureName;
				sub = name.find_last_of("/");
				name = name.substr(sub + 1);
			}
			else if (name2 == "")
			{
				name2 = textureName;
				sub = name2.find_last_of("/");
				name2 = name2.substr(sub + 1);
			}
			else
			{
				name3 = textureName;
				sub = name3.find_last_of("/");
				name3 = name3.substr(sub + 1);
			}


			if (name2 == "" && name3 == "")
				model.TextureName = name.c_str();
			else if (name3 == "")
				model.TextureName = name2.c_str();
			else
				model.TextureName = name3.c_str();

			delete[] textureName;
		}

		stream.read((char*)&numVerts, sizeof(numVerts));
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			VertexPositionUVNormal Vert;

			stream.read((char *)&Vert.pos.x, 4);
			stream.read((char *)&Vert.pos.y, 4);
			stream.read((char *)&Vert.pos.z, 4);
			Vert.pos.w = 1;

			stream.read((char *)&Vert.normal.x, 4);
			stream.read((char *)&Vert.normal.y, 4);
			stream.read((char *)&Vert.normal.z, 4);
			Vert.normal.w = 1;

			stream.read((char *)&Vert.uv.x, 4);
			stream.read((char *)&Vert.uv.y, 4);
			Vert.uv.z = 1;
			Vert.uv.w = 1;

			model.VertPosUVNorms.push_back(Vert);
		}

		stream.read((char*)&numTri, sizeof(numTri));
		for (unsigned int i = 0; i < numTri; ++i)
		{
			unsigned int temp = 0;

			stream.read((char*)&temp, sizeof(temp));
			model.VertexIndecies.push_back(temp);
			stream.read((char*)&temp, sizeof(temp));
			model.VertexIndecies.push_back(temp);
			stream.read((char*)&temp, sizeof(temp));
			model.VertexIndecies.push_back(temp);

			numIndex += 3;
		}

		delete meshName;
	}
}

//void LoadMeshFromFile(const char *path, Mesh m)
//{
//	std::ifstream Input;
//	Input.open(path, std::ios_base::binary);
//	if (Input.is_open())
//	{
//		int length;
//		char *str;
//		Input.read((char*)&length, sizeof(length));
//		str = new char[length];
//		Input.read(str, length);
//		std::string MeshName = str;
//
//		unsigned int TextureNameLength = 0;
//		Input.read((char*)&TextureNameLength, sizeof(TextureNameLength));
//		for (unsigned i = 0; i < TextureNameLength; i++)
//		{
//			unsigned int TextureNameLength2 = 0;
//			Input.read((char*)&TextureNameLength2, sizeof(TextureNameLength2));
//			char* TextureName = new char[TextureNameLength2];
//			Input.read(TextureName, TextureNameLength2);
//			std::string Name = TextureName;
//			unsigned int pos = Name.find_last_of("/\\");
//			Name = Name.substr(pos + 1);
//			m.TextureName = Name.c_str();
//		}
//
//		unsigned int StructLength = 0;
//		Input.read((char*)&StructLength, sizeof(StructLength));
//		for (unsigned int i = 0; i < StructLength; i++)
//		{
//			VertexPositionUVNormal vp;
//			Input.read((char*)&vp.pos.x, sizeof(vp.pos.x));
//			Input.read((char*)&vp.pos.y, sizeof(vp.pos.y));
//			Input.read((char*)&vp.pos.z, sizeof(vp.pos.z));
//			Input.read((char*)&vp.normal.x, sizeof(vp.normal.x));
//			Input.read((char*)&vp.normal.y, sizeof(vp.normal.y));
//			Input.read((char*)&vp.normal.z, sizeof(vp.normal.z));
//			Input.read((char*)&vp.uv.x, sizeof(vp.uv.x));
//			Input.read((char*)&vp.uv.y, sizeof(vp.uv.y));
//			m.VertPosUVNorms.push_back(vp);
//		}
//
//		unsigned int TriangleLength = 0;
//		Input.read((char*)&TriangleLength, sizeof(TriangleLength));
//		unsigned int point;
//		for (unsigned int i = 0; i < TriangleLength * 3; i++)
//		{
//			Input.read((char*)&point, sizeof(point));
//			m.VertexIndecies.push_back(point);
//		}
//
//		delete str;
//		Input.close();
//	}
//}

//void LoadMeshFromFile(const char *path, Mesh m)
//{
//	std::ifstream mFin;
//
//	mFin.open(path, std::ios_base::binary);
//
//	if (mFin.is_open())
//	{
//		unsigned int mLenghtname = 0;
//
//		mFin.read((char*)& mLenghtname, sizeof(mLenghtname));
//		char* mMeshName = new char[mLenghtname];
//		mFin.read(mMeshName, mLenghtname);
//		std::string mMeshnam;
//		mMeshnam = mMeshName;
//		delete[] mMeshName;
//		unsigned int mLenght3 = 0;
//
//		mFin.read((char*)&mLenght3, sizeof(mLenght3));
//
//
//		for (unsigned int i = 0; i < mLenght3; ++i)
//		{
//			unsigned int mLenght2 = 0;
//			mFin.read((char*)&mLenght2, sizeof(mLenght2));
//			std::string name;
//			char* temp = new char[mLenght2];
//			mFin.read(temp, mLenght2);
//			unsigned int j = 0;
//			while (temp[j] != NULL)
//			{
//				if (temp[j] == '\\' || temp[j] == '/')
//					name.clear();
//				else
//					name.push_back(temp[j]);
//
//				j++;
//			}
//			delete[] temp;
//			m.TextureName = name.c_str();
//		}
//
//		unsigned int mLenght4 = 0;
//		mFin.read((char*)&mLenght4, sizeof(mLenght4));
//
//		for (unsigned int i = 0; i < mLenght4; ++i)
//		{
//			VertexPositionUVNormal mTemp;
//
//
//			mFin.read((char*)(&mTemp.pos.x), sizeof(float));
//			mFin.read((char*)(&mTemp.pos.y), sizeof(float));
//			mFin.read((char*)(&mTemp.pos.z), sizeof(float));
//			mFin.read((char*)(&mTemp.normal.x), sizeof(float));
//			mFin.read((char*)(&mTemp.normal.y), sizeof(float));
//			mFin.read((char*)(&mTemp.normal.z), sizeof(float));
//			mFin.read((char*)(&mTemp.uv.x), sizeof(float));
//			mFin.read((char*)(&mTemp.uv.y), sizeof(float));
//
//			m.VertPosUVNorms.push_back(mTemp);
//		}
//		unsigned int mLenght5 = 0;
//
//		mFin.read((char*)&mLenght5, sizeof(mLenght5));
//
//		for (unsigned int i = 0; i < mLenght5; ++i)
//		{
//			unsigned int mTriangle = 0;
//			mFin.read((char*)(&mTriangle), sizeof(unsigned int));
//			m.VertexIndecies.push_back(mTriangle);
//			mFin.read((char*)(&mTriangle), sizeof(unsigned int));
//			m.VertexIndecies.push_back(mTriangle);
//			mFin.read((char*)(&mTriangle), sizeof(unsigned int));
//			m.VertexIndecies.push_back(mTriangle);
//			mTriangle += 3;
//
//		}
//		mFin.close();
//	}
//}