/************************************************
*              	   Material.h                   *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#pragma once

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Graphics/Texture.h>
#include <Graphics/Cubemap.h>
#include <Graphics/Shader.h>
#include <System/System.h>
#include <System/Log.h>
#include <System/Serializer.h>
#include <fstream>
#include <json.hpp>

namespace Columbus
{

	class C_Material
	{
	private:
		Vector4 mColor = Vector4(1, 1, 1, 1);
		Vector3 mDiffuse = Vector3(1, 1, 1);
		Vector3 mAmbient = Vector3(0.25, 0.25, 0.25);
		Vector3 mSpecular = Vector3(1, 1, 1);
		float mReflectionPower = 0.2;
		Texture* mTexture = nullptr;
		Texture* mSpecMap = nullptr;
		Texture* mNormMap = nullptr;
		Shader* mShader = nullptr;

		bool mDiscard = false;
		bool mLighting = true;

		Cubemap* mEnvReflection = nullptr;

		float mShininess = 32;

		int mTextureID = -1;
		int mSpecMapID = -1;
		int mNormMapID = -1;
	public:
		C_Material();
		C_Material(std::string aFile);

		void setColor(const Vector4 aColor);
		void setAmbient(const Vector3 aAmbient);
		void setDiffuse(const Vector3 aDiffuse);
		void setSpecular(const Vector3 aSpecular);
		void setShininess(const float aShininess);
		void setTexture(const Texture* aTexture);
		void setSpecMap(const Texture* aSpecMap);
		void setNormMap(const Texture* aNormMap);
		void setShader(const Shader* aShader);
		void setReflection(const Cubemap* aReflection);
		void setReflectionPower(const float aPower);
		void setDiscard(const bool aDiscard);
		void setLighting(const bool aLighting);

		Vector4 getColor() const;
		Vector3 getAmbient() const;
		Vector3 getDiffuse() const;
		Vector3 getSpecular() const;
		float getShininess() const;
		Texture* getTexture() const;
		Texture* getSpecMap() const;
		Texture* getNormMap() const;
		Shader* getShader() const;
		Cubemap* getReflection() const;
		float getReflectionPower() const;
		bool getDiscard() const;
		bool getLighting() const;

		int getTextureID() const;
		int getSpecMapID() const;
		int getNormMapID() const;

		bool saveToXML(std::string aFile) const;
		bool saveToJSON(std::string aFile) const;
		bool loadFromXML(std::string aFile);
		bool loadFromJSON(std::string aFile);

		~C_Material();
	};

}
