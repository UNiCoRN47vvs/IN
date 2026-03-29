#pragma once

#include "Actor_Components/Storage_Component.h"
#include "Inventory_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
class AItem_Actor_Master;
class UDA_Item_Master;
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UInventory_Component : public UStorage_Component
{
	GENERATED_BODY()
public:
	UInventory_Component();
	void Pick_Up_Item(AItem_Actor_Master* item_actor);

	TMulticastDelegate<void(UDA_Item_Master*, const int32)> On_Pick_Up_Item;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Server, Reliable) void S_Pick_Up_Item(AItem_Actor_Master* item_actor);
	UFUNCTION(Client, Unreliable) void C_On_Pick_Up_Item(UDA_Item_Master* da_item, const int32 item_count);
};
//------------------------------------------------------------------------------------------------------------