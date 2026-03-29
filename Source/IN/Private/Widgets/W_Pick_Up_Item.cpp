#include "Widgets/W_Pick_Up_Item.h"
#include "Components/Overlay.h"
#include "Actor_Components/Inventory_Component.h"
#include "Widgets/W_Pick_Up_Item_Box.h"
//------------------------------------------------------------------------------------------------------------
void UW_Pick_Up_Item::NativeConstruct()
{
	Super::NativeConstruct();

	Init_Self();
}
//------------------------------------------------------------------------------------------------------------
void UW_Pick_Up_Item::Init_Self()
{
	if (!GetOwningPlayerPawn())
	{
		TWeakObjectPtr<UW_Pick_Up_Item> weak_this = this;
		GetWorld()->GetTimerManager().SetTimerForNextTick([weak_this]() 
			{
				if (weak_this.IsValid())
					weak_this->Init_Self();
			});
		return;
	}

	UInventory_Component* inventory_component = GetOwningPlayerPawn()->FindComponentByClass<UInventory_Component>();

	if (inventory_component)
		inventory_component->On_Pick_Up_Item.AddUObject(this, &UW_Pick_Up_Item::On_Pick_Up_Item);
	else
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UW_Pick_Up_Item::Init_Self);
}
//------------------------------------------------------------------------------------------------------------
void UW_Pick_Up_Item::On_Pick_Up_Item(UDA_Item_Master* da_item, const int32 item_count)
{
	Pop_Up_Array.Add({da_item, item_count});

	if (!GetWorld()->GetTimerManager().TimerExists(Pop_Up_Handle))
		GetWorld()->GetTimerManager().SetTimer(Pop_Up_Handle, this, &UW_Pick_Up_Item::Create_Pop_Up, 1.0f, true, 0.0f);	
}
//------------------------------------------------------------------------------------------------------------
void UW_Pick_Up_Item::Create_Pop_Up()
{
	CHECK_PTR(Overlay);
	CHECK_PTR(W_Pick_Up_Item_Box_Class);

	if (!Pop_Up_Array.IsValidIndex(0))
	{
		GetWorld()->GetTimerManager().ClearTimer(Pop_Up_Handle);
		return;
	}

	UW_Pick_Up_Item_Box* pick_up_item_box = nullptr;
	FPop_Up_Info& pop_up_info = Pop_Up_Array[0];

	pick_up_item_box = CreateWidget<UW_Pick_Up_Item_Box>(this, W_Pick_Up_Item_Box_Class);
	CHECK_PTR(pick_up_item_box);

	Overlay->AddChild(pick_up_item_box);

	pick_up_item_box->Init_W_Pick_Up_Item_Box(pop_up_info.DA_Item->Item_Name, FText::AsNumber(pop_up_info.Item_Count), pop_up_info.DA_Item->Item_Icon);

	Pop_Up_Array.RemoveAt(0);
}
//------------------------------------------------------------------------------------------------------------
