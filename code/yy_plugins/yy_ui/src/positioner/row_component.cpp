#include "row_component.h"
#include "../item_transform_component.h"
NS_YY_BEGIN
void RowComponent::OnCreate(const VariantMap& args)
{

}

void RowComponent::OnDestroyed()
{

}

void RowComponent::OnEvent(const std::string& event, const YY::VarList& args)
{
	if (event == "AddChild")
	{
		if (args.GetCount() == 2 && args.GetType(1) == YVT_SINT32)
		{
			OnChildAdded(args.GetInt(1));
		}
	}
	else if (event == "RemoveChild")
	{
		if (args.GetCount() == 2 && args.GetType(1) == YVT_SINT32)
		{
			OnChildRemoved(args.GetInt(1));
		}
	}
}

void RowComponent::OnChildAdded(int index)
{
	YY_OBJECTID id = GetOwner()->GetChildByIndex(index);
	BaseObject* pObject = GetMgr()->Find(id);
	if (!pObject->IsInstanceOf("Entity"))
		return;

	Entity* pEntity = (Entity*)pObject;
	ItemTransformComponent* pTransform = (ItemTransformComponent*)pEntity->FindComponent("ItemTransformComponent");
	if (nullptr == pTransform)
		return;

	ItemTransformComponent* pOwnerTransform = (ItemTransformComponent*)GetOwner()->FindComponent("ItemTransformComponent");
	if (nullptr == pOwnerTransform)
		return;

	pTransform->SetTop(0);
	pTransform->SetLeft(0);
}

void RowComponent::OnChildRemoved(int index)
{

}

NS_YY_END