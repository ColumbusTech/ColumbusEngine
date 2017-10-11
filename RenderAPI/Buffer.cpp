/************************************************
*                  Buffer.cpp                   *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*             Nikolay(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#include <RenderAPI/Buffer.h>

namespace C
{

	//////////////////////////////////////////////////////////////////////////////
	//Constructor
	C_Buffer::C_Buffer()
	{
		C_GenBufferOpenGL(&mID);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Constructor 2
	C_Buffer::C_Buffer(const float* aData, const unsigned int aSize)
	{
		C_GenBufferOpenGL(&mID);
		setData(aData, aSize);
		compile();
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set vertices to buffer
	void C_Buffer::setData(const float* aData, const unsigned int aSize)
	{
		mData = aData;
		mSize = (unsigned int)aSize;
		mCount = aSize / sizeof(float) / 3;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Load buffer data to GPU
	void C_Buffer::compile()
	{
		C_BindBufferOpenGL(C_OGL_ARRAY_BUFFER, mID);
		C_BufferDataOpenGL(C_OGL_ARRAY_BUFFER, mSize, mData, C_OGL_STATIC_DRAW);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Bind vertex buffer
	void C_Buffer::bind()
	{
		C_BindBufferOpenGL(C_OGL_ARRAY_BUFFER, mID);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Unbind vertex buffer
	void C_Buffer::unbind()
	{
		C_BindBufferOpenGL(C_OGL_ARRAY_BUFFER, 0);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return packs count
	int C_Buffer::getCount()
	{
		return mCount;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Destructor
	C_Buffer::~C_Buffer()
	{
		C_DeleteBufferOpenGL(&mID);
	}

}