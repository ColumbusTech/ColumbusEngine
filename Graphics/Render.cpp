/************************************************
*                 Render.cpp                    *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*             Nikolay(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#include <Graphics/Render.h>

namespace C
{

	//////////////////////////////////////////////////////////////////////////////
	//Constructor
	C_Render::C_Render()
	{
		FB = new C_Framebuffer();
		TB = new C_Texture(NULL, 640, 480, true);
		RB = new C_Renderbuffer();

		FB->setTexture2D(C_FRAMEBUFFER_COLOR_ATTACH, TB->getID());
		RB->storage(C_RENDERBUFFER_DEPTH_24, 640, 480);
		FB->setRenderbuffer(C_FRAMEBUFFER_DEPTH_ATTACH, RB->getID());

		printf("%i\n", FB->check());

		mPostProcess = new C_Shader("Data/Shaders/post.vert", "Data/Shaders/post.frag");
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add mesh
	void C_Render::add(C_Mesh* aMesh)
	{
		mMeshes.push_back(aMesh);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Add particle emitter
	void C_Render::add(C_ParticleEmitter* aP)
	{
		mParticleEmitters.push_back(aP);
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set main camera
	void C_Render::setMainCamera(C_Camera* aCamera)
	{
		mCamera = aCamera;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set window size
	void C_Render::setWindowSize(C_Vector2 aWindowSize)
	{
		mWindowSize = aWindowSize;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Set skybox
	void C_Render::setSkybox(C_Skybox* aSkybox)
	{
		mSkybox = aSkybox;
	}
	//////////////////////////////////////////////////////////////////////////////
	//Render scene
	void C_Render::render()
	{
		//if (mSkybox != nullptr)
			//mSkybox->draw();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		/*for (size_t i = 0; i < mMeshes.size(); i++)
		{
			if (mCamera != nullptr)
				mMeshes[i]->setCamera(*mCamera);

			//mMeshes[i]->draw();
		}

		for (size_t i = 0; i < mParticleEmitters.size(); i++)
		{
			if (mCamera != nullptr)
				mParticleEmitters[i]->setCameraPos(mCamera->pos());

			//mParticleEmitters[i]->draw();
		}*/

		TB->load(NULL, mWindowSize.x, mWindowSize.y, true);
		RB->storage(C_RENDERBUFFER_DEPTH_24, mWindowSize.x, mWindowSize.y);

		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		FB->bind();
		glViewport(0, 0, mWindowSize.x, mWindowSize.y);

		glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//FB->prepare(C_Vector4(1, 1, 1, 0), mWindowSize);

		mSkybox->draw();

		mMeshes[0]->draw();
		mMeshes[1]->draw();

		mParticleEmitters[0]->draw();

		C_Framebuffer::unbind();

		TB->generateMipmap();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		C_Cubemap::unbind();
		C_Buffer::unbind();
		C_Texture::unbind();
		C_Shader::unbind();
		C_Framebuffer::unbind();
		C_Renderbuffer::unbind();

		mPostProcess->bind();
		mPostProcess->setUniform2f("uWindowSize", mWindowSize);

		TB->bind();
		drawQuad();
		C_Texture::unbind();

		C_Shader::unbind();
	}
	//////////////////////////////////////////////////////////////////////////////
	//Draw screen quad
	void C_Render::drawQuad()
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-1.0, -1.0);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(-1.0, 1.0);

			glTexCoord2f(0.0, 0.0);
			glVertex2f(-1.0, -1.0);

			glTexCoord2f(1.0, 0.0);
			glVertex2f(1.0, -1.0);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);
		glEnd();
	}
	//////////////////////////////////////////////////////////////////////////////
	//Destructor
	C_Render::~C_Render()
	{

	}

}
