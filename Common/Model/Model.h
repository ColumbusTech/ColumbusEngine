/************************************************
*                    Model.h                    *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   08.01.2018                  *
*************************************************/
#pragma once

#include <System/Assert.h>
#include <System/System.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace Columbus
{

	struct Vertex
	{
		Vector3 pos;
		Vector2 UV;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
	};

	bool ModelIsCMF(std::string FileName);
	bool ModelIsCMFMemory(uint8* FileData, uint64 FileSize);

	uint32 ModelLoadCMF(std::string FileName, std::vector<Vertex>& OutVertices);
	uint32 ModelLoadCMFCompressed(std::string FileName, std::vector<Vertex>& OutVertices);
	uint32 ModelLoadCMFFromMemory(uint8* FileData, uint64 FileSize, std::vector<Vertex>& OutVertices);
	uint32 ModelLoadCMFCompressedFromMemory(uint8* FileData, uint64 FileSize, std::vector<Vertex>& OutVertices);

	void ModelConvertCMFToCompressed(std::string SourceFileName, std::string DestinyFileName);

	class C_Model
	{
	private:
		std::vector<Vertex> Vertices;
		std::string FileName;
		bool Existance = false;
	public:
		C_Model();
		C_Model(std::string aFile);

		bool Load(std::string aFile);
		bool Save(std::string aFile) const;
		bool IsExist() const;
		bool Free();

		std::vector<Vertex> GetData() const;
		std::string GetFilename() const;

		~C_Model();
	};

}




