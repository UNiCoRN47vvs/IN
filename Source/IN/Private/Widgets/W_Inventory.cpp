#include "Widgets/W_Inventory.h"
#include "Components/Button.h"
#include "Actor_Components/Storage_Component.h"
#include "Widgets/W_Equipment.h"
//------------------------------------------------------------------------------------------------------------
void UW_Inventory::Init_Storage_Widget(UStorage_Component* component, UStorage_Component* target_drop_component)
{
	Super::Init_Storage_Widget(component, target_drop_component);

	CHECK_ALARM(W_Equipment, TEXT("W_Equipment is nullptr"));
	W_Equipment->Init_Equipment_Widget(component);
}
//------------------------------------------------------------------------------------------------------------
void UW_Inventory::Init_Equipment_W_Ptr(UW_Equipment* equipment)
{
	W_Equipment = equipment;

	Button_Equipment->OnClicked.AddUniqueDynamic(this, &UW_Inventory::Toggle_Equipment_Widget);
}
//------------------------------------------------------------------------------------------------------------
void UW_Inventory::Toggle_Equipment_Widget()
{
	PTR(W_Equipment)->Toggle_Equipment_Widget();
}
//------------------------------------------------------------------------------------------------------------