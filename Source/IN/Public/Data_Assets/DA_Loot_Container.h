#pragma once

#include "IN/IN.h"
#include "Engine/DataAsset.h"
#include "DA_Loot_Container.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Interact_Widget_Class;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Loot_Container : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(BlueprintReadOnly, Category = "DA_Loot_Container") double Drop_Total_Chance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DA_Loot_Container") float Extra_Drop_Chance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DA_Loot_Container") double Drop_Time_Refresh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DA_Loot_Container") TSoftObjectPtr<UDataTable> Drop_Table;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DA_Loot_Container") TObjectPtr<UDA_Interact_Widget_Class> DA_Interact_Widget_Class;
};
//------------------------------------------------------------------------------------------------------------