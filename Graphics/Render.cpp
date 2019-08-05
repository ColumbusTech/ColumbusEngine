#include <Graphics/Render.h>
#include <Graphics/Device.h>
#include <Graphics/RenderState.h>
#include <Scene/ComponentMeshRenderer.h>
#include <Scene/ComponentParticleSystem.h>
#include <Scene/Component.h>
#include <Math/Frustum.h>
#include <GL/glew.h>
#include <algorithm>
#include <cstddef>

#include <Profiling/Profiling.h>

#include <Graphics/OpenGL/ShaderOpenGL.h>
#include <Graphics/OpenGL/FramebufferOpenGL.h>

namespace Columbus
{
	Texture* Blob;

	RenderState State;

	Renderer::Renderer()
	{
		Blob = gDevice->CreateTexture();
		Blob->Load("Data/Textures/blob.png");

		BaseMSAA.ColorTexturesEnablement[0] = true;
		BaseMSAA.ColorTexturesEnablement[1] = true;
		BaseMSAA.ColorTexturesFormats[0] = TextureFormat::R11G11B10F; // Color texture
		BaseMSAA.ColorTexturesFormats[1] = TextureFormat::R11G11B10F; // Normal texture
		BaseMSAA.ColorTexturesMipmaps[0] = false;
		BaseMSAA.ColorTexturesMipmaps[1] = false;
		BaseMSAA.DepthTextureEnablement = true;

		Base.ColorTexturesEnablement[0] = true;
		Base.ColorTexturesEnablement[1] = true;
		Base.ColorTexturesFormats[0] = TextureFormat::R11G11B10F; // Color texture
		Base.ColorTexturesFormats[1] = TextureFormat::R11G11B10F; // Normal texture
		Base.ColorTexturesMipmaps[0] = false;
		Base.ColorTexturesMipmaps[1] = false;
		Base.DepthTextureEnablement = true;

		for (auto& post : Post)
		{
			post.ColorTexturesFormats[0] = TextureFormat::R11G11B10F;
			post.ColorTexturesFormats[1] = TextureFormat::R11G11B10F;
		}

		Final.ColorTexturesEnablement[0] = true;
	}

	void Renderer::SetViewport(const iVector2& Origin, const iVector2& Size)
	{
		ViewportOrigin = Origin;
		ViewportSize = Size;
	}

	void Renderer::SetMainCamera(const Camera& InCamera)
	{
		MainCamera = InCamera;
		State.SetMainCamera(MainCamera);
	}

	void Renderer::SetSky(Skybox* InSky)
	{
		Sky = InSky;
	}

	void Renderer::SetScene(Scene* InScn)
	{
		Scn = InScn;
	}

	void Renderer::SetDeltaTime(float Delta)
	{
		DeltaTime = Delta;
	}

	uint32 Renderer::GetPolygonsRendered() const
	{
		return PolygonsRendered;
	}

	uint32 Renderer::GetOpaqueObjectsRendered() const
	{
		return OpaqueObjectsRendered;
	}

	uint32 Renderer::GetTransparentObjectsRendered() const
	{
		return TransparentObjectsRendered;
	}

	void Renderer::SetRenderList(std::vector<SmartPointer<GameObject>>* List)
	{
		RenderList = List;
	}

	void Renderer::SetLightsList(std::vector<Light*>* List)
	{
		LightsList = List;
	}

	void Renderer::CompileLists()
	{
		PROFILE_CPU(ProfileModule::Culling);

		OpaqueObjects.clear();
		TransparentObjects.clear();

		LightsPairs.clear();

		Frustum ViewFrustum(MainCamera.GetViewProjection());

		for (uint32 i = 0; i < Scn->Lights.size(); i++)
		{
			LightsPairs.emplace_back(i, Scn->Lights[i]);
		}

		if (RenderList != nullptr)
		{
			uint32 Counter = 0;
			for (auto& Object : *RenderList)
			{
				if (Object->Enable)
				{
					ComponentMeshRenderer* MeshRenderer = (ComponentMeshRenderer*)Object->GetComponent(Component::Type::MeshRenderer);
					ComponentParticleSystem* ParticleSystem = (ComponentParticleSystem*)Object->GetComponent(Component::Type::ParticleSystem);

					if (MeshRenderer != nullptr)
					{
						auto Mesh = MeshRenderer->GetMesh();

						if (Mesh != nullptr)
						{
							if (ViewFrustum.Check(Mesh->GetBoundingBox() * Object->transform.GetMatrix()))
							{
								if (Object->material == nullptr) continue;
								Material& Mat = *Object->material;
								Mat.ReflectionMap = Sky->GetIrradianceMap();

								if (Mat.Transparent)
								{
									TransparentObjects.emplace_back(Mesh, Counter);
									CalculateLights(Object->transform.Position, TransparentObjects.back().Lights);
								}
								else
								{
									OpaqueObjects.emplace_back(Mesh, Counter);
									CalculateLights(Object->transform.Position, OpaqueObjects.back().Lights);
								}
							}
						}
					}

					if (ParticleSystem != nullptr)
					{
						auto Emitter = &ParticleSystem->Emitter;

						if (Emitter != nullptr)
						{
							//TransparentObjects.emplace_back(nullptr, Emitter, Object->transform, Object->material);
							TransparentObjects.emplace_back(Emitter, Counter);
						}
					}
				}

				Counter++;
			}
		}
	}

	void Renderer::SortLists()
	{
		auto OpaqueSorter = [&](const OpaqueRenderData& A, const OpaqueRenderData& B)->bool
		{
			return MainCamera.Pos.LengthSquare(Scn->Objects[A.Index]->transform.Position) < MainCamera.Pos.LengthSquare(Scn->Objects[B.Index]->transform.Position);
		};

		auto TransparentSorter = [&](const TransparentRenderData& A, const TransparentRenderData& B)->bool
		{
			return MainCamera.Pos.LengthSquare(Scn->Objects[A.Index]->transform.Position) > MainCamera.Pos.LengthSquare(Scn->Objects[B.Index]->transform.Position);
		};

		std::sort(OpaqueObjects.begin(), OpaqueObjects.end(), OpaqueSorter);
		std::sort(TransparentObjects.begin(), TransparentObjects.end(), TransparentSorter);
	}

	void Renderer::RenderOpaque()
	{
		PROFILE_GPU(ProfileModuleGPU::OpaqueStage);

		State.Clear();

		State.SetBlending(false);

		for (auto& Object : OpaqueObjects)
		{
			SmartPointer<GameObject>& GO = Scn->Objects[Object.Index];
			if (GO->material == nullptr) continue;
			Material& Mat = *GO->material;
			ShaderProgram* CurrentShader = Mat.ShaderProg;
			std::vector<Light*>& Lights = Scn->Lights;
				
			if (CurrentShader != nullptr)
			{
				State.SetCulling(Mat.Culling);
				State.SetDepthTesting(Mat.DepthTesting);
				State.SetDepthWriting(Mat.DepthWriting);
				State.SetShaderProgram(Mat.GetShader());
				State.SetMaterial(Mat, GO->transform.GetMatrix(), Sky);
				State.SetLights(Lights, Object.Lights);
				State.SetMesh(Object.Object);

				PolygonsRendered += Object.Object->Render();
				OpaqueObjectsRendered++;
			}
		}
	}

	void Renderer::RenderSky()
	{
		PROFILE_GPU(ProfileModuleGPU::SkyStage);

		if (Sky != nullptr)
		{
			Sky->Render();
		}
	}

	void Renderer::RenderTransparent()
	{
		PROFILE_GPU(ProfileModuleGPU::TransparentStage);

		if (RenderList != nullptr && TransparentObjects.size() != 0)
		{
			State.Clear();
			State.SetBlending(true);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (auto& Object : TransparentObjects)
			{
				SmartPointer<GameObject>& GO = Scn->Objects[Object.Index];
				if (GO->material == nullptr) continue;
				Material& Mat = *GO->material;

				ShaderProgramOpenGL* CurrentShader = (ShaderProgramOpenGL*)Mat.ShaderProg;
				Mesh* CurrentMesh = Object.MeshObject;

				std::vector<Light*>& Lights = Scn->Lights;

				if (Object.MeshObject != nullptr)
				{
					if (CurrentShader != nullptr)
					{
						State.SetDepthTesting(Mat.DepthTesting);
						State.SetShaderProgram(CurrentShader);
						State.SetMaterial(Mat, GO->transform.GetMatrix(), Sky);
						State.SetLights(Lights, Object.Lights);
						CurrentMesh->Bind();

						int32 Transparent = CurrentShader->GetFastUniform("Transparent");

						if (Mat.Culling == Material::Cull::No)
						{
							State.SetDepthWriting(true);
							CurrentShader->SetUniform(Transparent, 0);
							State.SetCulling(Material::Cull::No);
							CurrentMesh->Render();

							State.SetDepthWriting(false);
							CurrentShader->SetUniform(Transparent, 1);
							State.SetCulling(Material::Cull::Front);
							CurrentMesh->Render();
							State.SetCulling(Material::Cull::Back);
							PolygonsRendered += CurrentMesh->Render();
						}
						else
						{
							State.SetCulling(Mat.Culling);

							State.SetDepthWriting(true);
							CurrentShader->SetUniform(Transparent, 0);
							CurrentMesh->Render();

							State.SetDepthWriting(false);
							CurrentShader->SetUniform(Transparent, 1);
							PolygonsRendered += CurrentMesh->Render();
						}

						TransparentObjectsRendered++;

						CurrentMesh->Unbind();
					}
				}

				if (Object.Particles != nullptr)
				{
					State.SetShaderProgram(CurrentShader);
					ParticlesRender.Render(*Object.Particles, MainCamera, Mat);
				}
			}
		}

		State.SetDepthWriting(true);
	}

	void Renderer::RenderPostprocess()
	{

	}

	void Renderer::Render(RenderPass Pass)
	{
		if (RenderList != nullptr)
		{
			switch (Pass)
			{
			case RenderPass::Opaque:      RenderOpaque();      break;
			case RenderPass::Sky:         RenderSky();         break;
			case RenderPass::Transparent: RenderTransparent(); break;
			case RenderPass::Postprocess: RenderPostprocess(); break;
			}
		}

		State.SetCulling(Material::Cull::No);
	}

	void Renderer::CalculateLights(const Vector3& Position, int32(&Lights)[4])
	{
		static auto func = [&Position](const auto& A, const auto& B) -> bool
		{
			double ADistance = Math::Sqr(A.second->Pos.X - Position.X) + Math::Sqr(A.second->Pos.Y - Position.Y) + Math::Sqr(A.second->Pos.Z - Position.Z);
			double BDistance = Math::Sqr(B.second->Pos.X - Position.X) + Math::Sqr(B.second->Pos.Y - Position.Y) + Math::Sqr(B.second->Pos.Z - Position.Z);
			return ADistance < BDistance;
		};

		if (LightsPairs.size() >= 4)
		{
			std::sort(LightsPairs.begin(), LightsPairs.end(), func);
		}

		for (uint32 i = 0; i < 4 && i < LightsPairs.size(); i++)
		{
			Lights[i] = LightsPairs[i].first;
		}
	}

	void Renderer::RenderIcons()
	{
		auto Icon = gDevice->GetDefaultShaders()->Icon;

		static int IconTextureID = ((ShaderProgramOpenGL*)(Icon))->GetFastUniform("Texture");
		static int IconPosID = ((ShaderProgramOpenGL*)(Icon))->GetFastUniform("Pos");
		static int IconSizeID = ((ShaderProgramOpenGL*)(Icon))->GetFastUniform("Size");

		auto DrawIcon = [&](Vector4 Coords)
		{
			float InvDistance = 1.0f / Coords.XYZ().Length(MainCamera.Pos) * 0.5f;
			InvDistance = Math::Clamp(InvDistance, 0.05f, 0.5f);
			Coords = MainCamera.GetViewProjection() * Coords;

			if (Coords.W > 0.0f)
			{
				Coords /= Coords.W;

				float Aspect = (float)ContextSize.X / (float)ContextSize.Y;
				Vector2 Size = InvDistance / Vector2(Aspect, 1);

				((ShaderProgramOpenGL*)(Icon))->SetUniform(IconPosID, Coords.XY());
				((ShaderProgramOpenGL*)(Icon))->SetUniform(IconSizeID, Size);
				Quad.Render();
			}
		};

		((ShaderProgramOpenGL*)(Icon))->Bind();

		State.SetBlending(true);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		((ShaderProgramOpenGL*)(Icon))->SetUniform(IconTextureID, (TextureOpenGL*)gDevice->GetDefaultTextures()->IconSun, 0);
		for (const auto& Elem : Scn->Lights)
		{
			if (Elem != nullptr)
				if (Elem->Type == Light::Directional)
					DrawIcon(Vector4(Elem->Pos, 1));
		}

		((ShaderProgramOpenGL*)(Icon))->SetUniform(IconTextureID, (TextureOpenGL*)gDevice->GetDefaultTextures()->IconLamp, 0);
		for (const auto& Elem : Scn->Lights)
		{
			if (Elem != nullptr)
				if (Elem->Type == Light::Point)
					DrawIcon(Vector4(Elem->Pos, 1));
		}

		((ShaderProgramOpenGL*)(Icon))->SetUniform(IconTextureID, (TextureOpenGL*)gDevice->GetDefaultTextures()->IconFlashlight, 0);
		for (const auto& Elem : Scn->Lights)
		{
			if (Elem != nullptr)
				if (Elem->Type == Light::Spot)
					DrawIcon(Vector4(Elem->Pos, 1));
		}

		((ShaderProgramOpenGL*)(Icon))->SetUniform(IconTextureID, (TextureOpenGL*)gDevice->GetDefaultTextures()->IconAudio, 0);
		for (const auto& Elem : Scn->Audio.Mixer.Sources)
		{
			if (Elem != nullptr)
				DrawIcon(Vector4(Elem->Position, 1));
		}

		((ShaderProgramOpenGL*)(Icon))->SetUniform(IconTextureID, (TextureOpenGL*)gDevice->GetDefaultTextures()->IconParticles, 0);
		for (const auto& Elem : TransparentObjects)
		{
			if (Elem.Particles != nullptr)
				DrawIcon(Vector4(Scn->Objects[Elem.Index]->transform.Position, 1));
		}

		((ShaderProgramOpenGL*)(Icon))->Unbind();
	}

	void Renderer::Render()
	{
		struct _UBO_Data
		{
			struct
			{
				float min, max;
				float speedUp, speedDown;
				float deltaTime;
				float pad[59]; // because nvidia requires 256-byte alignment
			} autoExposure;

			struct
			{
				float exposure, gamma;
				int type;
				int ae_enable;
				float pad[60];
			} tonemap;

			struct
			{
				Vector3 color;
				float intensity;
				Vector2 center;
				float smoothness;
				float radius;
				float pad[56];
			} vignette;
		} uboData;

		uboData.autoExposure.min = AutoExposure.Min;
		uboData.autoExposure.max = AutoExposure.Max;
		uboData.autoExposure.speedUp = AutoExposure.SpeedUp;
		uboData.autoExposure.speedDown = AutoExposure.SpeedDown;
		uboData.autoExposure.deltaTime = DeltaTime;

		uboData.tonemap.exposure = Exposure;
		uboData.tonemap.gamma = Gamma;
		uboData.tonemap.type = (int)Tonemapping;
		uboData.tonemap.ae_enable = AutoExposure.Enabled;

		uboData.vignette.color = Vignette.Color;
		uboData.vignette.intensity = Vignette.Intensity;
		uboData.vignette.center = Vignette.Center;
		uboData.vignette.smoothness = Vignette.Smoothness;
		uboData.vignette.radius = Vignette.Radius;

		static BufferOpenGL UBO(BufferType::Uniform, {
			sizeof(uboData),
			BufferUsage::Write,
			BufferCpuAccess::Stream
		});

		UBO.Load(&uboData);

		auto defaultShaders = gDevice->GetDefaultShaders();

		auto ScreenSpaceShader = static_cast<ShaderProgramOpenGL*>(defaultShaders->ScreenSpace);
		auto TonemapShader     = static_cast<ShaderProgramOpenGL*>(defaultShaders->Tonemap);
		auto MSAAShader        = static_cast<ShaderProgramOpenGL*>(defaultShaders->ResolveMSAA);
		auto FXAAShader        = static_cast<ShaderProgramOpenGL*>(defaultShaders->FXAA);
		auto EditorToolsShader = static_cast<ShaderProgramOpenGL*>(defaultShaders->EditorTools);

		if (ContextSize.X == 0) ContextSize.X = 1;
		if (ContextSize.Y == 0) ContextSize.Y = 1;

		PolygonsRendered = 0;
		OpaqueObjectsRendered = 0;
		TransparentObjectsRendered = 0;

		CompileLists();
		SortLists();

		PROFILE_GPU(ProfileModuleGPU::GPU);

		bool IsFXAA = false;
		bool IsMSAA = false;

		switch (Antialiasing)
		{
		case AntialiasingType::No: break;
		case AntialiasingType::FXAA: IsFXAA = true; break;
		case AntialiasingType::MSAA_2X:  BaseMSAA.Multisampling = 2;  IsMSAA = true; break;
		case AntialiasingType::MSAA_4X:  BaseMSAA.Multisampling = 4;  IsMSAA = true; break;
		case AntialiasingType::MSAA_8X:  BaseMSAA.Multisampling = 8;  IsMSAA = true; break;
		case AntialiasingType::MSAA_16X: BaseMSAA.Multisampling = 16; IsMSAA = true; break;
		case AntialiasingType::MSAA_32X: BaseMSAA.Multisampling = 32; IsMSAA = true; break;
		}

		GLuint BuffersAll[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		GLuint BuffersFirst[] = { GL_COLOR_ATTACHMENT0 };

		// RENDERING
		//
		//
		// Render to the MSAA buffer or not
		if (IsMSAA)
			BaseMSAA.Bind({0}, {0}, ContextSize);
		else
			Base.Bind({0}, {0}, ContextSize);

		RenderOpaque();
		glDrawBuffers(1, BuffersFirst);
		RenderSky();
		glDrawBuffers(2, BuffersAll);
		RenderTransparent();

		if (DrawGrid)
		{
			glDrawBuffers(1, BuffersFirst);

			State.SetBlending(true);
			State.SetDepthWriting(true);
			State.SetDepthTesting(Material::DepthTest::LEqual);

			EditorToolsShader->Bind();
			EditorToolsShader->SetUniform("ViewProjection", false, MainCamera.GetViewProjection());
			EditorToolsShader->SetUniform("Color", Vector4(Vector3(0.5f), 1.0f));
			EditorToolsShader->SetUniform("CameraPos", MainCamera.Pos);
			_Grid.Draw();
			EditorToolsShader->Unbind();
		}

		//
		//
		// RENDERING

		if (IsMSAA)
		{
			Base.Bind({0}, {0}, ContextSize);

			// Resolve RT0 (HDR Color)
			MSAAShader->Bind();
			MSAAShader->SetUniform("BaseTexture", (TextureOpenGL*)BaseMSAA.ColorTextures[0], 0);
			MSAAShader->SetUniform("Samples", (int)BaseMSAA.Multisampling);
			Quad.Render();
			MSAAShader->Unbind();

			// Resolve RT1 (Normals) and Depth
			glBindFramebuffer(GL_READ_FRAMEBUFFER, ((FramebufferOpenGL*)BaseMSAA.FB)->ID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ((FramebufferOpenGL*)Base.FB)->ID);

			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			glBlitFramebuffer(0, 0, ContextSize.X, ContextSize.Y, 0, 0, ContextSize.X, ContextSize.Y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

			glBlitFramebuffer(0, 0, ContextSize.X, ContextSize.Y, 0, 0, ContextSize.X, ContextSize.Y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}

		Base.ColorTexturesMipmaps[0] = true;

		// !!!!!!!!!!!
		if (AutoExposure.Enabled)
		{
			Base.Mipmaps();
		}

		State.SetBlending(false);
		State.SetDepthWriting(false);
		State.SetCulling(Material::Cull::No);

		Texture* FinalTex = Base.ColorTextures[0];

		auto autoExposureTexture = AutoExposure.Draw(Exposure,
			Base.ColorTextures[0], ContextSize,
			UBO, offsetof(_UBO_Data, autoExposure), sizeof(uboData.autoExposure));

		Bloom.Draw(FinalTex, ContextSize, Post);

		// Final stage
		{
			PROFILE_GPU(ProfileModuleGPU::FinalStage);

			Final.Bind({}, {}, ContextSize);

			TonemapShader->Bind();
			TonemapShader->SetUniform("u_BaseTexture", (TextureOpenGL*)FinalTex, 0);
			TonemapShader->SetUniform("u_AETexture", (TextureOpenGL*)autoExposureTexture, 1);
			UBO.BindRange(0, offsetof(_UBO_Data, tonemap), sizeof(uboData.tonemap));
			Quad.Render();
			TonemapShader->Unbind();

			if (IsFXAA)
			{
				Post[0].ColorTexturesEnablement[0] = true;
				Post[0].Bind({}, {}, ContextSize);

				FXAAShader->Bind();
				FXAAShader->SetUniform("BaseTexture", (TextureOpenGL*)Final.ColorTextures[0], 0);
				FXAAShader->SetUniform("Resolution", (Vector2)ContextSize);
				Quad.Render();

				Final.FB->Bind();

				ScreenSpaceShader->Bind();
				ScreenSpaceShader->SetUniform("BaseTexture", (TextureOpenGL*)Post[0].ColorTextures[0], 0);
				Quad.Render();
			}

			if (DrawIcons)
			{
				RenderIcons();
			}

			if (Vignette.Enabled)
			{
				State.SetBlending(true);
				Vignette.Draw(UBO, offsetof(_UBO_Data, vignette), sizeof(uboData.vignette));
			}

			Final.Unbind();
		}

		// Lens flare rendering test, I will use it in the future

		/*static int LensFlareShaderTextureID = ((ShaderProgramOpenGL*)(LensFlareShader))->GetFastUniform("Texture");

		Vector4 Coords(Vector3(0), 1);
		Coords = MainCamera.GetViewProjection() * Coords;

		if (Coords.W != 0.0f)
			Coords /= Coords.W;

		float Aspect = (float)ContextSize.X / (float)ContextSize.Y;

		LensFlareShader->Bind();
		((ShaderProgramOpenGL*)(LensFlareShader))->SetUniform(LensFlareShaderTextureID, (TextureOpenGL*)Blob, 0);
		Quad.Render(Coords.XY(),  0.2f / Vector2(Aspect, 1));
		LensFlareShader->Unbind();*/
	}

	Texture* Renderer::GetFramebufferTexture() const
	{
		return Final.ColorTextures[0];
	}

	Renderer::~Renderer()
	{
		delete Blob;
		//delete EditorIconLamp;
	}

}


