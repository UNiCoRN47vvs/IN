#pragma once

#include "Actor_Components/Items/Item_Comp_Master.h"
#include "Item_Comp_Ranged.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Equippable;
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UItem_Comp_Ranged : public UItem_Comp_Master
{
	GENERATED_BODY()
public:
	UItem_Comp_Ranged();
	virtual void Init_Item_Comp(UDA_Item_Master* item_da, USkeletalMeshComponent* owner_mesh, UItem_Master* item) override;
protected:
	void Create_Comp_Ranged(UDA_Item_Equippable* item_equippable_da, USkeletalMeshComponent* owner_mesh);
};
//------------------------------------------------------------------------------------------------------------