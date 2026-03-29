#include "Widgets/W_Equipment.h"
#include "Blueprint/WidgetTree.h"
#include "Actor_Components/Equipment_Component.h"
#include "Data_Assets/DA_Item_Equippable.h"
#include "Widgets/W_Equipment_Slot.h"
//------------------------------------------------------------------------------------------------------------
void UW_Equipment::Init_Equipment_Widget(UStorage_Component* component)
{
	CHECK_PTR(component);

	UW_Equipment_Slot* equipment_slot = nullptr;
	TArray<UWidget*> all_widgets = {};
	Is_Opened_VB_Main = false;
	Is_Opened_HB_Fast_Slot = false;

	WidgetTree->GetAllWidgets(all_widgets);
	
	for (UWidget* widget : all_widgets)
	{
		equipment_slot = Cast<UW_Equipment_Slot>(widget);
		if (equipment_slot)
		{
			equipment_slot->Init_Slot(component, 0);
			Equipment_Slots.Add(equipment_slot);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_Equipment::Toggle_Equipment_Widget()
{
	PlayAnimation(Toggle_Animation_VB_Main, 0.0f, 1, Is_Opened_VB_Main ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	
	Is_Opened_VB_Main = !Is_Opened_VB_Main;
}
//------------------------------------------------------------------------------------------------------------
