/************************************************
*              	  GameObejct.cpp                *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*                Nika(Columbus) Red             *
*                   12.11.2017                  *
*************************************************/
#include <Scene/GameObject.h>

namespace Columbus
{

	GameObject::GameObject()
	{

	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void GameObject::setName(const std::string aName)
	{
		mName = static_cast<std::string>(aName);
	}
	//////////////////////////////////////////////////////////////////////////////
	std::string GameObject::getName() const
	{
		return mName;
	}
	//////////////////////////////////////////////////////////////////////////////
	void GameObject::addChild(GameObject* aChild)
	{
		mChildren.push_back(aChild);
	}
	//////////////////////////////////////////////////////////////////////////////
	void GameObject::addComponent(Component* aComponent)
	{
		mComponents.push_back(aComponent);
	}
	//////////////////////////////////////////////////////////////////////////////
	void GameObject::setTransform(Transform aTransform)
	{
		transform = aTransform;
	}
	//////////////////////////////////////////////////////////////////////////////
	Transform GameObject::getTransform() const
	{
		return transform;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	void GameObject::update()
	{
		transform.update();

		for (auto Comp : mComponents)
			Comp->update(static_cast<float>(mTimer.elapsed()));

		for (auto Child : mChildren)
			Child->update();
	}
	//////////////////////////////////////////////////////////////////////////////
	void GameObject::render()
	{
		for (auto Comp : mComponents)
			Comp->render(transform);

		for (auto Child : mChildren)
			Child->render();

		mTimer.reset();
	}
	//////////////////////////////////////////////////////////////////////////////
	bool GameObject::hasComponent(std::string aName)
	{
		for (auto Comp : mComponents)
			if (Comp->getType() == aName)
				return true;

		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	Component* GameObject::getComponent(std::string aName)
	{
		for (auto Comp : mComponents)
			if (Comp->getType() == aName)
				return Comp;

		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	GameObject::~GameObject()
	{

	}

}

