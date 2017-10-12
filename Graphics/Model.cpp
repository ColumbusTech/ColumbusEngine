/************************************************
*              	   Model.cpp                    *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*             Nikolay(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#include <Graphics/Model.h>

namespace C
{

	//////////////////////////////////////////////////////////////////////////////
	//Constructor
	C_Mesh::C_Mesh(std::vector<C_Vertex> aVert)
	{
		mPos = C_Vector3(0, 0, 0);
		mRot = C_Vector3(0, 0, 0);
		mScale = C_Vector3(1, 1, 1);
		mPivot = C_Vector3(0, 0, 0);

		mVert = aVert;

		std::vector<float> v;

		for (size_t i = 0; i < mVert.size(); i++)
		{
			v.push_back(mVert[i].pos.x);
			v.push_back(mVert[i].pos.y);
			v.push_back(mVert[i].pos.z);
		}

		if (v.size() > 0)
			buf = new C_Buffer(v.data(), v.size() * sizeof(float));
		v.clear();

		std::vector<float> t;

		for (size_t i = 0; i < mVert.size(); i++)
		{
			t.push_back(mVert[i].UV.x);
			t.push_back(mVert[i].UV.y);
		}

		if(t.size() > 0)
			tbuf = new C_Buffer(t.data(), t.size() * sizeof(float));
		t.clear();

		std::vector<float> n;

		for (size_t i = 0; i < mVert.size(); i++)
		{
			n.push_back(mVert[i].normal.x);
			n.push_back(mVert[i].normal.y);
			n.push_back(mVert[i].normal.z);
		}

		if (n.size() > 0)
			nbuf = new C_Buffer(n.data(), n.size() * sizeof(float));
		n.clear();

		std::vector<float> tang;

		for (size_t i = 0; i < mVert.size(); i++)
		{
			tang.push_back(mVert[i].tangent.x);
			tang.push_back(mVert[i].tangent.y);
			tang.push_back(mVert[i].tangent.z);
		}

		if (tang.size() > 0)
			tangbuf = new C_Buffer(tang.data(), tang.size() * sizeof(float));
		tang.clear();

		std::vector<float> bitang;

		for (size_t i = 0; i < mVert.size(); i++)
		{
			tang.push_back(mVert[i].bitangent.x);
			tang.push_back(mVert[i].bitangent.y);
			tang.push_back(mVert[i].bitangent.z);
		}

		if (bitang.size() > 0)
			bitangbuf = new C_Buffer(bitang.data(), bitang.size() * sizeof(float));
		bitang.clear();
	}
	//////////////////////////////////////////////////////////////////////////////
	//Constructor 2
	C_Mesh::C_Mesh(std::string aFile)
	{
		Importer::C_Importer importer;
		if(importer.load(aFile))
		{

		}
	}
	//////////////////////////////////////////////////////////////////////////////
	//Constructor 3
  C_Mesh::C_Mesh()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//Draw mesh
	void C_Mesh::draw()
	{
		if (buf == nullptr)
			return;

		C_Buffer* buffers[5] = {buf, tbuf, nbuf, tangbuf, bitangbuf};
		unsigned int indices[5] = {0, 1, 2, 3, 4};
		unsigned int strides[5] = {3, 2, 3, 3, 3};

		for (int i = 0; i < 5; i++)
			if (buffers[i] != nullptr)
				buffers[i]->bind(indices[i], C_OGL_FALSE, strides[i] * sizeof(float));

		if (mMat.getShader() != nullptr)
		{
			mMat.getShader()->bind();

			setShaderMatrices();
			setShaderMaterial();
			setShaderLightAndCamera();
			setShaderTextures();
		}


		C_DrawArraysOpenGL(C_OGL_TRIANGLES, 0, mVert.size());
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set camera
	void C_Mesh::setCamera(C_Camera aCamera)
	{
		mCamera = aCamera;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set mesh position
	void C_Mesh::setPos(C_Vector3 aPos)
	{
		mPos = aPos;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set mesh rotation
	void C_Mesh::setRot(C_Vector3 aRot)
	{
		mRot = aRot;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set mesh scale
	void C_Mesh::setScale(C_Vector3 aScale)
	{
		mScale = aScale;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add position to current
	void C_Mesh::addPos(C_Vector3 aPos)
	{
		mPos += aPos;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add rotation to current
	void C_Mesh::addRot(C_Vector3 aRot)
	{
		mRot += aRot;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add scale to current
	void C_Mesh::addScale(C_Vector3 aScale)
	{
		mScale += aScale;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set parent mesh
	void C_Mesh::setParent(C_Mesh* aParent)
	{
		mParent = aParent;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add child mesh
	void C_Mesh::addChild(C_Mesh* aChild)
	{
		if (aChild == nullptr)
			return;

		mChilds.push_back(aChild);
		aChild->setParent(this);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set pivot point
	void C_Mesh::setPivot(C_Vector3 aPivot)
	{
		mPivot = aPivot;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Return pivot point
	C_Vector3 C_Mesh::getPivot()
	{
		return mPivot;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add position to pivot
	void C_Mesh::addPivot(C_Vector3 aPivot)
	{
		mPivot += aPivot;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Sets texture\cubemap as uniform in shader
	void C_Mesh::setShaderTextures()
	{
		C_Texture* textures[3] = {mMat.getTexture(), mMat.getSpecMap(), mMat.getNormMap()};
		C_Cubemap* cubemap = mMat.getReflection();
		std::string unifs[3] = {"uMaterial.diffuseMap", "uMaterial.specularMap", "uMaterial.normalMap"};
		unsigned int indices[3] = {0, 1, 3};

		for (int i = 0; i < 3; i++)
		{
			if (textures[i] != nullptr)
			{
				mMat.getShader()->setUniform1i(unifs[i].c_str(), indices[i]);
				textures[i]->sampler2D(indices[i]);
			} else
			{
				mMat.getShader()->setUniform1i(unifs[i].c_str(), indices[i]);
				C_DeactiveTextureOpenGL(C_OGL_TEXTURE0);
			}
		}

		if (cubemap != nullptr)
		{
			mMat.getShader()->setUniform1i("uReflectionMap", 2);
			cubemap->samplerCube(2);
		} else
		{
			mMat.getShader()->setUniform1i("uReflectionMap", 2);
			C_DeactiveCubemapOpenGL(C_OGL_TEXTURE2);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	//Calculate and transfer matrices as uniform in shader
	void C_Mesh::setShaderMatrices()
	{
		if (mParent != nullptr)
			mMatrix = mParent->mMatrix;
		else
			mMatrix = glm::mat4(1.0f);

		mMatrix = glm::translate(mMatrix, mPivot.toGLM() + mPos.toGLM());
		mMatrix = glm::scale(mMatrix, mScale.toGLM());
		mMatrix = glm::rotate(mMatrix, C_DegToRads(mRot.z), glm::vec3(0, 0, 1));
		mMatrix = glm::rotate(mMatrix, C_DegToRads(mRot.x), glm::vec3(1, 0, 0));
		mMatrix = glm::rotate(mMatrix, C_DegToRads(mRot.y), glm::vec3(0, 1, 0));
		mMatrix = glm::translate(mMatrix, -(mPivot.toGLM() + mPos.toGLM()));
		mMatrix = glm::translate(mMatrix, mPos.toGLM());

		mNormalMatrix = glm::inverse(glm::transpose(mMatrix));

		mMat.getShader()->setUniformMatrix("uModel", glm::value_ptr(mMatrix));
		mMat.getShader()->setUniformMatrix("uView", glm::value_ptr(C_GetViewMatrix()));
		mMat.getShader()->setUniformMatrix("uProjection", glm::value_ptr(C_GetProjectionMatrix()));
		mMat.getShader()->setUniformMatrix("uNormal", glm::value_ptr(mNormalMatrix));
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set all material data as uniform in shader
	void C_Mesh::setShaderMaterial()
	{
		C_Vector4 matcol = mMat.getColor();
		C_Vector3 matamb = mMat.getAmbient();
		C_Vector3 matdif = mMat.getDiffuse();
		C_Vector3 matspc = mMat.getSpecular();

		float MaterialUnif[14] =
		{
			matcol.x, matcol.y, matcol.z, matcol.w,
			matamb.x, matamb.y, matamb.z,
			matdif.x, matdif.y, matdif.z,
			matspc.x, matspc.y, matspc.z,
			mMat.getReflectionPower()
		};

		mMat.getShader()->setUniformArrayf("MaterialUnif", MaterialUnif, 14);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set all lights and camera data as unniform in shader
	void C_Mesh::setShaderLightAndCamera()
	{
		C_Vector3 lightCol = C_Vector3(1, 1, 1);
		C_Vector3 lightPos = mCamera.pos();
		C_Vector3 lightDir = mCamera.direction();
		float lightType = 2;
		float lightConst = 1;
		float lightLinear = 0.09;
		float lightQuadratic = 0.032;
		float lightInnerAngle = glm::radians(12.5);
		float lightOuterAngle = glm::radians(17.5);

		float LightUnif[15] =
		{
			lightCol.x, lightCol.y, lightCol.z,
			lightPos.x, lightPos.y, lightPos.z,
			lightDir.x, lightDir.y, lightDir.z,
			lightType,
			lightConst, lightLinear, lightQuadratic,
			lightInnerAngle, lightOuterAngle
		};

		mMat.getShader()->setUniformArrayf("LightUnif", LightUnif, 15);
		mMat.getShader()->setUniform3f("uCamera.pos", mCamera.pos());
	}
	//////////////////////////////////////////////////////////////////////////////
	//Destructor
	C_Mesh::~C_Mesh()
	{

	}

}
