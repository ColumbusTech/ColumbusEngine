/************************************************
*                  Texture.cpp                  *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#include <Graphics/Texture.h>
#include <Common/Image/Image.h>

namespace Columbus
{

	//////////////////////////////////////////////////////////////////////////////
	Texture::Texture()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	Texture::Texture(std::string aPath, bool aSmooth)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	Texture::Texture(const char* aData, const int aW, const int aH, bool aSmooth)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void Texture::load(std::string aPath, bool aSmooth)
	{

	}

	//////////////////////////////////////////////////////////////////////////////
	void Texture::load(const char* aData, const int aW, const int aH, bool aSmooth)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void Texture::loadDepth(const char* aData, const int aW, const int aH, bool aSmooth)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void Texture::setConfig(TextureConfig aConfig)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void Texture::setSmooth(const bool aSmooth)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void Texture::setAnisotropy(const unsigned int aAnisotropy)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	TextureConfig Texture::getConfig()
	{
		return mConfig;
	}
	//////////////////////////////////////////////////////////////////////////////
	size_t Texture::getSize()
	{
		if (!mImage.isExist()) return 0;

		return mWidth * mHeight * (mBPP / 8);
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool Texture::save(std::string aFile, size_t aQuality)
	{
		if (!mImage.isExist())
		{ Log::error("Texture didn't saved: " + aFile);  return false; }

		int type = E_IMAGE_FORMAT_PNG;

		switch (mImage.getBPP())
		{
		case 3: type = E_IMAGE_FORMAT_JPG; break;
		case 4: type = E_IMAGE_FORMAT_PNG; break;
		}

		ImageSave(aFile, mImage.getWidth(), mImage.getHeight(), mImage.getBPP(), mImage.getData(), type, aQuality);

		Log::success("Texture successfully saved: " + aFile);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void Texture::bind()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void Texture::unbind()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void Texture::sampler2D(int a)
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	void Texture::generateMipmap()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	std::string Texture::getType()
	{
		return "Texture";
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	Texture::~Texture()
	{

	}

}
