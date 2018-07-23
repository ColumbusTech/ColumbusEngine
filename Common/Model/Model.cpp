/************************************************
*                    Model.h                    *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   08.01.2018                  *
*************************************************/
#include <Common/Model/Model.h>

namespace Columbus
{

	C_Model::C_Model() :
		Existance(false)
	{

	}
	//////////////////////////////////////////////////////////////////////////////	
	C_Model::C_Model(std::string InFileName)
	{
		Load(InFileName);
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::Load(std::string InFileName)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::Save(std::string InFileName) const
	{
		if (!IsExist()) return false;

		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::IsExist() const
	{
		return Existance;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::Free()
	{
		Vertices.clear();
		FileName.clear();
		Existance = false;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////////
	std::vector<Vertex> C_Model::GetData() const
	{
		return Vertices;
	}
	//////////////////////////////////////////////////////////////////////////////
	std::string C_Model::GetFilename() const
	{
		return FileName;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	C_Model::~C_Model()
	{
		Free();
	}

}




