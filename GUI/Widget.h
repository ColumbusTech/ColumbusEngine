#pragma once

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

#include <GUI/IO.h>
#include <Graphics/Shader.h>
#include <RenderAPI/APIOpenGL.h>
#include <RenderAPI/Buffer.h>

namespace Columbus
{

	namespace GUI
	{

		class C_Widget
		{
		protected:
			C_Vector2 mPos = C_Vector2(0, 0);
			C_Vector2 mSize = C_Vector2(200, 100);

			int mState = 0;
			C_IO mIO;
		public:
			C_Widget() {}

			void setPos(const C_Vector2 aPos)
			{ mPos = static_cast<C_Vector2>(aPos); }

			void setSize(const C_Vector2 aSize)
			{ mSize = static_cast<C_Vector2>(aSize); }

			void setIO(const C_IO& aIO)
			{ mIO = static_cast<C_IO>(aIO); }

			C_Vector2& getPos()
			{ return mPos; }

			C_Vector2& getSize()
			{ return mSize; }

			virtual void update() = 0;
			virtual void draw() = 0;

			~C_Widget() {}
		};

	}

}
