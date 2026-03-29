#include "Actor_Components/Drop_Component.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Data_Assets/DA_Loot_Container.h"
#include "Data_Assets/DA_Item_Master.h"
#include "Structs/Drop_Item_Row.h"
#include "Structs/Drop_Item.h"
//------------------------------------------------------------------------------------------------------------
UDrop_Component::UDrop_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}
//------------------------------------------------------------------------------------------------------------
void UDrop_Component::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
void UDrop_Component::Generate_Drop(UDA_Loot_Container* da_loot_container, const int32 storage_size)
{
	CHECK_PTR(da_loot_container);

	int32 count_stot_for_item = storage_size;
	float total_chanse = 0.0f;
	float current_chance = 0.0f;
	float random_chanse = 0.0f;
	float random_second_drop_chanse = da_loot_container->Extra_Drop_Chance; //Шанс втого лута в дропе и так далее.
	FDrop_Item_Info item_drop = {};
	TArray<FDrop_Item_Row*> all_items_row = {};
	TArray<FDrop_Item_Info> drop_array = {};

	if (!da_loot_container->Drop_Table.Get())
	{
		TWeakObjectPtr<UDrop_Component> weak_this = this;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(da_loot_container->Drop_Table.ToSoftObjectPath(), [weak_this, da_loot_container, storage_size]()
			{
				if(weak_this.IsValid())
					weak_this->Generate_Drop(da_loot_container, storage_size);
			});

		return;
	}

	da_loot_container->Drop_Table.Get()->GetAllRows<FDrop_Item_Row>(TEXT("Context"), all_items_row);

	while (true) //Цикл на дополнительный дроп если будет прок шанса
	{
		random_chanse = FMath::RandRange(0.0, da_loot_container->Drop_Total_Chance);

		for (const FDrop_Item_Row* row : all_items_row) //Поиск предмета исходя из random_chanse
		{
			if (!row)
				continue;

			current_chance += row->Drop_Chance_Weight;

			if (random_chanse <= current_chance)
			{
				item_drop.Drop_Count = FMath::RandRange(row->Drop_Count_Min, row->Drop_Count_Max);
				item_drop.Drop_DA_Item = row->Drop_DA_Item;

				drop_array.Add(item_drop);

				item_drop = {};
				break;
			}
		}

		current_chance = 0.0f;

		if (random_second_drop_chanse < FMath::RandRange(0.0f, 100.0f)) //Проверка на дополнительный дроп предмета
			break;

		--count_stot_for_item;

		if (count_stot_for_item <= 0)
			break;
	}

	Send_Drop(drop_array);
}
//------------------------------------------------------------------------------------------------------------
void UDrop_Component::Send_Drop(const TArray<FDrop_Item_Info>& drop_array)
{
	for(const FDrop_Item_Info& item_info : drop_array)
	{
		if (!item_info.Drop_DA_Item.Get())
		{
			TWeakObjectPtr<UDrop_Component> weak_this = this;
			TArray<FSoftObjectPath> paths = {};

			UAssetManager::GetStreamableManager().RequestAsyncLoad(paths, [weak_this, drop_array]()
				{
					if (weak_this.IsValid())
						weak_this->Send_Drop(drop_array);
				});

			return;
		}
	}
	FDrop_Item item = {};
	TArray<FDrop_Item> drop_items = {};
	for (const FDrop_Item_Info& item_info : drop_array)
	{
		item.Drop_Count = item_info.Drop_Count;
		item.Drop_DA_Item = item_info.Drop_DA_Item.Get();
		drop_items.Add(item);
	}

	On_Send_Drop.ExecuteIfBound(drop_items);
}
//------------------------------------------------------------------------------------------------------------
