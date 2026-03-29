#pragma once

#include "Data_Assets/DA_Item_Master.h"
#include "DA_Item_Interactive.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Item_Interactive : public UDA_Item_Master
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Interactive") float Item_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Interactive") FName Socket_Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Interactive") UAnimMontage* Montage_Interactive;
};
//------------------------------------------------------------------------------------------------------------