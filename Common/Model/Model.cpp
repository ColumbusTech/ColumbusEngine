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
		mExistance(false)
	{

	}
	//////////////////////////////////////////////////////////////////////////////	
	C_Model::C_Model(const std::string aFile)
	{
		load(aFile);
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::load(const std::string aFile)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::save(const std::string aFile) const
	{
		if (!isExist()) return false;

		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::isExist() const
	{
		return mExistance;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool C_Model::freeData()
	{
		mVertices.clear();
		mFilename.clear();
		mExistance = false;
	}
	//////////////////////////////////////////////////////////////////////////////
	std::vector<C_Vertex> C_Model::getData() const
	{
		return mVertices;
	}
	//////////////////////////////////////////////////////////////////////////////
	std::string C_Model::getFilename() const
	{
		return mFilename;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	C_Model::~C_Model()
	{
		freeData();
	}

}




