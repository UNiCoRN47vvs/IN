#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Interact_Widget_Class.generated.h"
//------------------------------------------------------------------------------------------------------------
class UW_Container;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Interact_Widget_Class : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Master") TSubclassOf<UW_Container> Interact_Widget_Class;
};
//------------------------------------------------------------------------------------------------------------