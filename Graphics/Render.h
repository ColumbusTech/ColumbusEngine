#pragma once

#include <Graphics/Mesh.h>
#include <Graphics/Camera.h>
#include <Graphics/Skybox.h>
#include <Graphics/Light.h>
#include <Graphics/Particles/ParticleEmitterCPU.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/PostEffect.h>
#include <Scene/GameObject.h>

#include <Graphics/ScreenQuad.h>
#include <Graphics/ParticlesRenderer.h>

#include <map>

namespace Columbus
{

	class Renderer
	{
	protected:
		struct OpaqueRenderData
		{
			Mesh* Object;
			Transform ObjectTransform;
			Material ObjectMaterial;

			OpaqueRenderData(Mesh* InObject, const Transform& InTransform, const Material& InMaterial) :
				Object(InObject),
				ObjectTransform(InTransform),
				ObjectMaterial(InMaterial) {}
		};

		struct TransparentRenderData
		{
			Mesh* MeshObject;
			ParticleEmitterCPU* Particles;

			Transform ObjectTransform;
			Material ObjectMaterial;

			TransparentRenderData(Mesh* InMesh, ParticleEmitterCPU* CPU, const Transform& InTransform, const Material& InMaterial) :
				MeshObject(InMesh),
				Particles(CPU),
				ObjectTransform(InTransform),
				ObjectMaterial(InMaterial) {}
		};
	protected:
		std::map<uint32, SmartPointer<GameObject>>* RenderList;
		std::vector<Light*>* LightsList;

		std::vector<OpaqueRenderData> OpaqueObjects;
		std::vector<TransparentRenderData> TransparentObjects;
		
		iVector2 ContextSize;
		Camera MainCamera;
		Skybox* Sky = nullptr;

		PostEffect BaseEffect;
		PostEffect BloomBrightPass;
		PostEffect BloomHorizontalBlurPass;
		PostEffect BloomVerticalBlurPass;
		PostEffect BloomFinalPass;

		ScreenQuad Quad;
		ParticlesRenderer ParticlesRender;
	public:
		enum class Stage
		{
			Opaque,
			Sky,
			Transparent
		};
	public:
		float Exposure = 1.3f;
		float Gamma = 1.5f;

		float BloomTreshold = 0.8f;
		float BloomIntensity = 0.5f;
		float BloomRadius = 1.0f;
		int BloomIterations = 2;
	private:
		void RenderBloom();
	public:
		Renderer();
		
		void SetContextSize(const iVector2& Size);
		void SetMainCamera(const Camera& InCamera);
		void SetSky(Skybox* InSky);

		virtual void SetRenderList(std::map<uint32, SmartPointer<GameObject>>* List);
		virtual void SetLightsList(std::vector<Light*>* List);
		virtual void CompileLists();
		virtual void SortLists();

		virtual void RenderOpaqueStage();
		virtual void RenderSkyStage();
		virtual void RenderTransparentStage();
		virtual void Render(Stage RenderStage);
		virtual void Render();

		~Renderer();
	};

}


