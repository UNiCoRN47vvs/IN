#pragma once

#include "Data_Assets/DA_Item_Master.h"
#include "Enums/Stat.h"
#include "DA_Item_Consumable.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Item_Consumable : public UDA_Item_Master
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DA_Item_Consumable") TMap<EStat_Name, float> Stat_Changes;
};
//------------------------------------------------------------------------------------------------------------