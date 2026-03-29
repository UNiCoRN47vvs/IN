#pragma once

#include "Data_Assets/DA_Item_Interactive.h"
#include "DA_Item_Fist.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Item_Fist : public UDA_Item_Interactive
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Fist") float Fist_Radius_Size;
};
//------------------------------------------------------------------------------------------------------------