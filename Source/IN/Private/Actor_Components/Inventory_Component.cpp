#include "Actor_Components/Inventory_Component.h"
#include "Data_Assets/DA_Item_Master.h"
#include "Items/Item_Master.h"
#include "Actors/Items/Item_Actor_Master.h"
//------------------------------------------------------------------------------------------------------------
UInventory_Component::UInventory_Component()
{
}

//------------------------------------------------------------------------------------------------------------
void UInventory_Component::BeginPlay()
{
	Super::BeginPlay();

	Is_Init = true;
}
//------------------------------------------------------------------------------------------------------------
void UInventory_Component::Pick_Up_Item(AItem_Actor_Master* item_actor)
{
	S_Pick_Up_Item(item_actor);
}
//------------------------------------------------------------------------------------------------------------
void UInventory_Component::S_Pick_Up_Item_Implementation(AItem_Actor_Master* item_actor)
{
	CHECK_PTR(item_actor);

	UItem_Master* item_master = nullptr;
	UDA_Item_Master* da_item = item_actor->Get_Item_DA();
	int32 item_count = 0;
	TArray<int32> empty_slots_indexes = {};

	CHECK_PTR(da_item);

	for (int32 i = 0; i < Storage.Num(); i++)
	{
		if (!Storage[i].Item)
		{
			empty_slots_indexes.Add(i);
			continue;
		}

		item_count = item_actor->Item_Count;

		Storage[i].Item_Combine(da_item, item_actor->Item_Count);

		item_count -= item_actor->Item_Count; //Узнаем сколько было поднято 

		if(item_count > 0)
			C_On_Pick_Up_Item(da_item, item_count);

		if (item_actor->Item_Count <= 0)
		{
			item_actor->Destroy();
			return;
		}
	}

	for (int32 index : empty_slots_indexes)
	{
		ensureMsgf(da_item->Item_Object_Class, TEXT("Class is empty!"));

		item_master = NewObject<UItem_Master>(this, da_item->Item_Object_Class);
		if (!item_master)
		{
			ALARM_LOG;
			continue;
		}
		item_master->Init_Item(0, da_item, index);

		item_count = item_actor->Item_Count;

		Storage[index].Item = item_master;
		Storage[index].Item_Combine(da_item, item_actor->Item_Count);

		item_count -= item_actor->Item_Count; //Узнаем сколько было поднято 

		if (item_count > 0)
			C_On_Pick_Up_Item(da_item, item_count);

		if (item_actor->Item_Count <= 0)
		{
			item_actor->Destroy();
			return;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void UInventory_Component::C_On_Pick_Up_Item_Implementation(UDA_Item_Master* da_item, const int32 item_count)
{
	On_Pick_Up_Item.Broadcast(da_item, item_count);
}
//------------------------------------------------------------------------------------------------------------
