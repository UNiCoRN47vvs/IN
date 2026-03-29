#pragma once

#include "CoreMinimal.h"
#include "Drop_Item_Row.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Master;
//------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct IN_API FDrop_Item_Row : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop_Item_Row") int32 Drop_Count_Min;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop_Item_Row") int32 Drop_Count_Max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop_Item_Row") float Drop_Chance_Weight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop_Item_Row") TSoftObjectPtr<UDA_Item_Master> Drop_DA_Item;
};
//------------------------------------------------------------------------------------------------------------