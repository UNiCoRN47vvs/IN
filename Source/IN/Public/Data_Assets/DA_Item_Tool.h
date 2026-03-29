#pragma once

#include "Data_Assets/DA_Item_Interactive.h"
#include "DA_Item_Tool.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Item_Tool : public UDA_Item_Interactive
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Tool") TSoftObjectPtr<UStaticMesh> Tool_Static_Mesh;
};
//------------------------------------------------------------------------------------------------------------