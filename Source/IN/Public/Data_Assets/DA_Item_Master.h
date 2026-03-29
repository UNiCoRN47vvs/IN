#pragma once

#include "IN/IN.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Enums/Interact_Item_Type.h"
#include "DA_Item_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
class UItem_Master;
class UItem_Comp_Master;
class AItem_Actor_Master;
class UDA_Interact_Widget_Class;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Item_Master : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") int32 Item_Count_Max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") FText Item_Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") FGameplayTag Item_Tag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TSoftObjectPtr<UTexture2D> Item_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TSubclassOf<UItem_Master> Item_Object_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TSubclassOf<UItem_Comp_Master> Item_Comp_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TSubclassOf<AItem_Actor_Master> Item_Actor_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TObjectPtr<UDA_Interact_Widget_Class> DA_Interact_Widget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TArray<EInteract_Item_Type> Item_Interact_Types;

};
//------------------------------------------------------------------------------------------------------------