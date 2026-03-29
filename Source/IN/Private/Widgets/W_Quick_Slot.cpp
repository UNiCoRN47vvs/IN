#include "Widgets/W_Quick_Slot.h"
#include "Components/TextBlock.h"
#include "Actor_Components/Quick_Bar_Component.h"
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot::NativePreConstruct()
{
	Super::NativePreConstruct();
}
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot::NativeDestruct()
{
	Super::NativeDestruct();

	if (On_Use_Item)
		On_Use_Item->RemoveAll(this);
}
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot::Init_Slot(UStorage_Component* storage_component, int32 storage_index)
{
	CHECK_PTR(storage_component);

	UQuick_Bar_Component* quick_bar_component = nullptr;

	Storage_Component = storage_component;

	quick_bar_component = Cast<UQuick_Bar_Component>(storage_component);

	CHECK_PTR(quick_bar_component);

	On_Use_Item = &quick_bar_component->On_Use_Item;

	On_Use_Item->AddUObject(this, &UW_Quick_Slot::Use_Anim_Play);

	Super::Init_Slot(storage_component, storage_index);

	Item_Button_Number->SetText(FText::AsNumber(Storage_Index + 1));
}
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot::Use_Anim_Play(int32 index)
{
	if(Storage_Index == index)
		PlayAnimation(Use_Animation);
}
//------------------------------------------------------------------------------------------------------------
