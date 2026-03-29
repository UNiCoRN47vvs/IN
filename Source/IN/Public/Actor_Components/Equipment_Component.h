#pragma once

#include "Actor_Components/Storage_Component.h"
#include "GameplayTagContainer.h"
#include "Equipment_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UEquipment_Component : public UStorage_Component
{
	GENERATED_BODY()
public:
	UEquipment_Component();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment_Component") TArray<FGameplayTag> Equipment_Slot_Tags;

};
//------------------------------------------------------------------------------------------------------------