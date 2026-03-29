#pragma once

#include "Actor_Components/Items/Item_Comp_Master.h"
#include "Item_Comp_Melee.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Tool;
class UDA_Item_Fist;
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UItem_Comp_Melee : public UItem_Comp_Master
{
	GENERATED_BODY()
public:
	UItem_Comp_Melee();
	virtual void Init_Item_Comp(UDA_Item_Master* item_da, USkeletalMeshComponent* owner_mesh, UItem_Master* item) override;
protected:
	void Create_Comp_Melee(UDA_Item_Tool* item_tool_da, USkeletalMeshComponent* owner_mesh);
	void Create_Comp_Fist(UDA_Item_Fist* item_fist_da, USkeletalMeshComponent* owner_mesh);
};
//------------------------------------------------------------------------------------------------------------