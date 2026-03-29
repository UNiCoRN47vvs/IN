#include "Widgets/W_Container.h"
#include "Widgets/W_Storage.h"
//------------------------------------------------------------------------------------------------------------
void UW_Container::Init_Container_Widget(UStorage_Component* component_player, UStorage_Component* component_interact)
{
	if(W_Inventory)
		W_Inventory->Init_Storage_Widget(component_player, component_interact);

	if (W_Interact)
		W_Interact->Init_Storage_Widget(component_interact, component_player);
}
//------------------------------------------------------------------------------------------------------------
void UW_Container::Close_Container_Widget()
{
	if (W_Inventory)
		W_Inventory->Close_Widget();

	if (W_Interact)
		W_Interact->Close_Widget();
}
//------------------------------------------------------------------------------------------------------------
void UW_Container::Init_W_Container(UW_Storage* w_inventory, UW_Storage* w_interact)
{
	W_Inventory = w_inventory;
	W_Interact = w_interact;
}
//------------------------------------------------------------------------------------------------------------
