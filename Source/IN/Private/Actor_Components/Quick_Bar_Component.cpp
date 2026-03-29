#include "Actor_Components/Quick_Bar_Component.h"
#include "Net/UnrealNetwork.h"
//------------------------------------------------------------------------------------------------------------
UQuick_Bar_Component::UQuick_Bar_Component()
{
	Storage_Size = 8;
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::BeginPlay()
{
	Super::BeginPlay();

	Is_Init = true;

	On_Hand_Item_Changed.AddUObject(this, &UQuick_Bar_Component::Hand_Item_Changed);
	On_Slot_Drag_Triggered.AddUObject(this, &UQuick_Bar_Component::Slot_Drag_Triggered);
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	On_Hand_Item_Changed.RemoveAll(this);
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::Rep_Update_Storage()
{
	Update_Storage();
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::Hand_Item_Changed(UDA_Item_Master* item_master, UItem_Interactive* item_interactive, int32 index)
{
	Current_Hand_Item_Index = index;
	Current_Hand_Item_DA = item_master;
	Current_Hand_Item = item_interactive;
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::Slot_Drag_Triggered(int32 index)
{
	if (index == Current_Hand_Item_Index)
	{
		On_Hand_Item_Changed.Broadcast(Current_Hand_Item_DA, Current_Hand_Item, -1);

		Current_Hand_Item_Index = -1;
		Current_Hand_Item_DA = nullptr;
		Current_Hand_Item = nullptr;
	}
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::Quick_Slot_Item_Use(int32 number)
{
	const int32 index = number - 1;

	CHECK_ALARM(Storage.IsValidIndex(index), TEXT("Index is not valid!"));

	if (!Storage[index].Item)
		return;

	Storage[index].Item->Item_Use();

	if (Storage[index].Item->Item_Count <= 0)
		Storage[index].Item_Destroy();

	if (GetNetMode() != NM_DedicatedServer)
	{
		//C_Update_Slot(index);
		On_Use_Item.Broadcast(index);
		return;
	}

	C_Use_Item_Update(index);
}
//------------------------------------------------------------------------------------------------------------
void UQuick_Bar_Component::C_Use_Item_Update_Implementation(int32 index)
{
	On_Use_Item.Broadcast(index);

	//Update_Slot(index);
}
//------------------------------------------------------------------------------------------------------------
