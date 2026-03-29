#include "Items/Item_Master.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Actor_Components/Storage_Component.h"
#include "Data_Assets/DA_Item_Master.h"
//------------------------------------------------------------------------------------------------------------
UWorld* UItem_Master::GetWorld() const
{
	const UObject* outer = GetOuter();
	if (outer)
		return outer->GetWorld();

	return nullptr;
}
//------------------------------------------------------------------------------------------------------------
void UItem_Master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItem_Master, Item_Count);
	DOREPLIFETIME(UItem_Master, Item_DA);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Master::Init_Item(const int32 item_count, UDA_Item_Master* item_da, const int32 item_index)
{
	Item_Count = item_count;
	Item_DA = item_da;
	Item_Index = item_index;

	Update_Slot();
}
//------------------------------------------------------------------------------------------------------------
bool UItem_Master::Item_Combine(UDA_Item_Master* item_da, int32& item_count)
{
	if (item_da != Item_DA || !Item_DA)
		return false;

	int32 item_count_max = Item_DA->Item_Count_Max;
	int32 item_count_sum = Item_Count + item_count;

	if (item_count_sum <= item_count_max)
	{
		item_count = 0;
		Item_Count = item_count_sum;
	}
	else
	{
		item_count = item_count_sum - item_count_max;
		Item_Count = item_count_max;
	}

	Update_Slot();

	return true;
}
//------------------------------------------------------------------------------------------------------------
void UItem_Master::Update_Slot()
{
	UStorage_Component* storage_component = nullptr;

	storage_component = Cast<UStorage_Component>(GetOuter());
	CHECK_PTR(storage_component);
	storage_component->C_Update_Slot(Item_Index, Item_Count, Item_DA);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Master::Update_Slot_Destroy()
{
	if (IsRunningDedicatedServer())
	{
		UStorage_Component* storage_component = nullptr;

		storage_component = Cast<UStorage_Component>(GetOuter());
		CHECK_PTR(storage_component);
		storage_component->C_Update_Slot(Item_Index, 0, nullptr);
	}
}
//------------------------------------------------------------------------------------------------------------
void UItem_Master::On_Rep_Item_Count()
{
	UStorage_Component* storage_component = nullptr;

	storage_component = Cast<UStorage_Component>(GetOuter());
	CHECK_PTR(storage_component);
	storage_component->C_Update_Slot(Item_Index, Item_Count, Item_DA);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Master::Item_Use()
{

}
//------------------------------------------------------------------------------------------------------------