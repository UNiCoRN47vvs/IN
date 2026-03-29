#pragma once

#include "IN/IN.h"
#include "Components/ActorComponent.h"
#include "Drop_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Loot_Container;
class UDA_Item_Master;
struct FDrop_Item;
//------------------------------------------------------------------------------------------------------------
struct FDrop_Item_Info
{
	int32 Drop_Count;
	TSoftObjectPtr<UDA_Item_Master> Drop_DA_Item;
};
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IN_API UDrop_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDrop_Component();
	void Generate_Drop(UDA_Loot_Container* da_loot_container, const int32 storage_size);

	TDelegate<void(const TArray<FDrop_Item>&)> On_Send_Drop;
protected:
	virtual void BeginPlay() override;

	void Send_Drop(const TArray<FDrop_Item_Info>& drop_array);
};
//------------------------------------------------------------------------------------------------------------