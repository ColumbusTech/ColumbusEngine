#pragma once

#include <cstdio>
#include <vector>
#include <string>

#include <System/File.h>
#include <System/System.h>
#include <System/Log.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Graphics/ShaderBuilder.h>
#include <Core/Containers/Array.h>

namespace Columbus
{

	struct ShaderAttribute
	{
		std::string Name;
		uint32 Value;

		ShaderAttribute(std::string InName, uint32 InValue) :
			Name(InName), Value(InValue) {}
	};

	class Shader
	{
	protected:
		ShaderBuilder Builder;
		std::vector<ShaderAttribute> Attributes;

		std::string VertShaderPath;
		std::string FragShaderPath;

		std::string VertShaderSource;
		std::string FragShaderSource;

		bool Loaded = false;
		bool Compiled = false;
	public:
		Shader() : Compiled(false), Loaded(false) {}
		//Shader(std::string InVert, std::string InFrag)  : Compiled(false), Loaded(false) { Load(InVert, InFrag); }

		virtual bool Load(std::string aVert, std::string aFrag) { return false; }
		virtual bool Compile() { return false; }

		bool IsLoaded() const { return Loaded; }
		bool IsCompiled() const { return Compiled; }

		virtual void Bind() const {}
		virtual void Unbind() const {}

		void AddAttribute(std::string InName, uint32 InValue) { Attributes.emplace_back(InName, InValue); }

		virtual void AddUniform(std::string Name) {}
		virtual void SetUniform1i(std::string Name, int Value) const {}
		virtual void SetUniform1f(std::string Name, float Value) const {}
		virtual void SetUniform2f(std::string Name, Vector2 Value) const {}
		virtual void SetUniform3f(std::string Name, Vector3 Value) const {}
		virtual void SetUniform4f(std::string Name, Vector4 Value) const {}
		virtual void SetUniformMatrix(std::string Name, const float* Value) const {}
		virtual void SetUniformArrayf(std::string Name, const float* Array, uint32 Size) const {}

		virtual ~Shader() {}
	};

	class ShaderStage
	{
	protected:
		ShaderBuilder Builder;
		ShaderType Type;

		std::string ShaderPath;
		std::string ShaderSource;

		bool Loaded;
		bool Compiled;
	public:
		ShaderStage() {}

		virtual bool IsValid() const { return false; }
		virtual bool Load(std::string InPath, ShaderType InType) { return false; }
		virtual bool Compile() { return false; }

		bool IsLoaded() const { return Loaded; }
		bool IsCompiled() const  { return Compiled; }
		ShaderType GetType() const { return Type; }

		virtual ~ShaderStage() {}
	};

	class ShaderProgram
	{
	protected:
		std::vector<ShaderAttribute> Attributes;
		std::vector<ShaderStage*> Stages;

		bool Compiled;
	public:
		ShaderProgram() {}

		bool IsLoaded() const
		{
			for (auto& Stage : Stages)
			{
				if (!Stage->IsLoaded())
				{
					return false;
				}
			}

			return true;
		}

		bool IsCompiled() const
		{
			if (!Compiled) return false;

			for (auto& Stage : Stages)
			{
				if (!Stage->IsCompiled())
				{
					return false;
				}
			}

			return true;
		}

		void AddAttribute(std::string InName, uint32 InValue) { Attributes.emplace_back(InName, InValue); }

		virtual void Bind() const {}
		virtual void Unbind() const {}

		virtual void AddStage(ShaderStage* Stage) {}
		virtual bool Compile() { return false; }

		virtual void AddUniform(std::string Name) {}
		virtual void SetUniform1i(std::string Name, int Value) const {}
		virtual void SetUniform1f(std::string Name, float Value) const {}
		virtual void SetUniform2f(std::string Name, Vector2 Value) const {}
		virtual void SetUniform3f(std::string Name, Vector3 Value) const {}
		virtual void SetUniform4f(std::string Name, Vector4 Value) const {}
		virtual void SetUniformMatrix(std::string Name, const float* Value) const {}
		virtual void SetUniformArrayf(std::string Name, const float* Array, uint32 Size) const {}

		virtual ~ShaderProgram() {}
	};

}
