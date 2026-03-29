#include "Data_Assets/DA_Loot_Container.h"
#include "Structs/Drop_Item_Row.h"
#include "Data_Assets/DA_Item_Master.h"
//------------------------------------------------------------------------------------------------------------
void UDA_Loot_Container::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	TArray<FDrop_Item_Row*> all_items_row = {};

	if (!Drop_Table.Get())
	{
		Drop_Table.LoadSynchronous();
		if (!Drop_Table.Get())
			return;
	}

	Drop_Table.Get()->GetAllRows<FDrop_Item_Row>(TEXT("Context"), all_items_row);
	Drop_Total_Chance = 0.0f;

	for (const FDrop_Item_Row* row : all_items_row)
	{
		if (!row)
			continue;

		Drop_Total_Chance += row->Drop_Chance_Weight;
	}
}
//------------------------------------------------------------------------------------------------------------