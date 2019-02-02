#include "item_generator_component.h"



void ItemGeneratorComponent::OnCreate(const VariantMap& args)
{

}

Item* ItemGeneratorComponent::Create()
{
	if (nullptr == m_pItem)
		return nullptr;

	return (Item*)m_pItem->Duplicate();
}