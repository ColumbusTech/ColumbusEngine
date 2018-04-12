#pragma once

#include <Physics/PhysicsShape.h>
#include <Math/Vector3.h>

namespace Columbus
{

	class PhysicsShapeCylinder : public PhysicsShape
	{
	private:
		Vector3 Size;
	public:
		PhysicsShapeCylinder(Vector3 Size)
		{
			mShape = new btCylinderShape(btVector3(Size.x * 0.5f, Size.y * 0.5f, Size.z * 0.5f));
			this->Size = Size;
		}
		/*
		* Returns size of cylinder collision shape
		* @return Vector3: Size of cylinder collision shape
		*/
		Vector3 GetSize() const
		{
			return Size;
		}
	};

}

