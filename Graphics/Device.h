#pragma once

#include <Graphics/Shader.h>
#include <Graphics/Cubemap.h>
#include <Graphics/Texture.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshInstanced.h>
#include <Graphics/Framebuffer.h>

namespace Columbus
{

	class Device
	{
	public:
		Device();

		virtual ShaderStage* CreateShaderStage() const;
		virtual ShaderProgram* CreateShaderProgram() const;

		virtual Cubemap* createCubemap() const;
		virtual Cubemap* createCubemap(std::string aPath) const;
		virtual Cubemap* createCubemap(std::array<std::string, 6> aPath) const;

		virtual Texture* createTexture() const;
		virtual Texture* createTexture(std::string aPath, bool aSmooth = true) const;
		virtual Texture* createTexture(const char* aData, const int aW, const int aH, bool aSmooth = true) const;

		virtual Mesh* createMesh() const;
		virtual Mesh* createMesh(std::vector<Vertex> aVert) const;
		virtual Mesh* createMesh(std::vector<Vertex> aVert, Material aMat) const;

		virtual MeshInstanced* CreateMeshInstanced() const;

		virtual Framebuffer* createFramebuffer() const;

		virtual ~Device();
	};

	extern Device* gDevice;

}










