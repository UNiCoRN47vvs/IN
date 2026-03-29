#include "Items/Item_Consumable.h"
#include "Actor_Components/Stat_Player_Component.h"
#include "Data_Assets/DA_Item_Consumable.h"
//------------------------------------------------------------------------------------------------------------
void UItem_Consumable::Item_Use()
{
	Super::Item_Use();

	UActorComponent* actor_component = nullptr;
	UStat_Player_Component* player_component = nullptr;
	UDA_Item_Consumable* item_consumable = Cast<UDA_Item_Consumable>(Item_DA);
	CHECK_PTR(item_consumable);

	actor_component = Cast<UActorComponent>(GetOuter());
	CHECK_PTR(actor_component);
	CHECK_PTR(actor_component->GetOwner());

	player_component = actor_component->GetOwner()->FindComponentByClass<UStat_Player_Component>();
	CHECK_PTR(player_component);

	for (const TPair<EStat_Name, float>& stat : item_consumable->Stat_Changes)
	{
		player_component->Add_Stat_Value(stat.Key, stat.Value);
	}

	--Item_Count;

	Update_Slot();
}
//------------------------------------------------------------------------------------------------------------
