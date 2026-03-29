#pragma once

#include "Data_Assets/DA_Item_Interactive.h"
#include "DA_Item_Equippable.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Item_Equippable : public UDA_Item_Interactive
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Equipment") TSoftObjectPtr<USkeletalMesh> Equippable_Skeletal_Mesh;
};
//------------------------------------------------------------------------------------------------------------