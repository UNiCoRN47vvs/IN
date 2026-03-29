#include "Items/Item_Interactive.h"
#include "Actor_Components/Quick_Bar_Component.h"
//------------------------------------------------------------------------------------------------------------
UQuick_Bar_Component* UItem_Interactive::Get_Quick_Bar_Component()
{
	return Quick_Bar_Component ? Quick_Bar_Component : Quick_Bar_Component = Cast<UQuick_Bar_Component>(GetOuter());
}
//------------------------------------------------------------------------------------------------------------
void UItem_Interactive::Item_Use()
{
	CHECK_PTR(Get_Quick_Bar_Component());

	Quick_Bar_Component->On_Hand_Item_Changed.Broadcast(Item_DA, this, Item_Index);
}
//------------------------------------------------------------------------------------------------------------