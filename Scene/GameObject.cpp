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
	
	void GameObject::SetName(std::string Name)
	{
		this->Name = Name;
	}
	
	std::string GameObject::GetName() const
	{
		return Name;
	}
	
	void GameObject::AddChild(GameObject* Child)
	{
		Children.push_back(SmartPointer<GameObject>(Child));
	}
	
	void GameObject::AddComponent(Component* InComponent)
	{
		Components.push_back(SmartPointer<Component>(InComponent));
	}
	
	void GameObject::SetTransform(Transform Transform)
	{
		transform = Transform;
	}
	
	Transform& GameObject::GetTransform()
	{
		return transform;
	}
	
	void GameObject::Update()
	{
		transform.Update();

		for (auto& Comp : Components)
		{
			Comp->Update(static_cast<float>(mTimer.elapsed()));
		}

		for (auto& Child : Children)
		{
			Child->Update();
		}

		mTimer.reset();
	}
	
	void GameObject::Render()
	{
		for (auto& Comp : Components)
		{
			Comp->Render(transform);
		}

		for (auto& Child : Children)
		{
			Child->Render();
		}
	}
	
	bool GameObject::HasComponent(Component::Type Type)
	{
		for (auto& Comp : Components)
		{
			if (Comp->GetType() == Type)
			{
				return true;
			}
		}

		return false;
	}
	
	Component* GameObject::GetComponent(Component::Type Type)
	{
		for (auto& Comp : Components)
		{
			if (Comp->GetType() == Type)
			{
				return Comp.get();
			}
		}

		return nullptr;
	}
	
	GameObject::~GameObject()
	{

	}

}

