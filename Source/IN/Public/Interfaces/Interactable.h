#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"
//------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct IN_API FInteract_Info
{
	GENERATED_BODY()

	UPROPERTY() FText Name_Action = FText::FromString("Interact");
	UPROPERTY() FText Name_Owner = FText::FromString("Actor_Interact");
	UPROPERTY() FText Name_Button = FText::FromString("F");
};
//------------------------------------------------------------------------------------------------------------
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};
//------------------------------------------------------------------------------------------------------------
class IN_API IInteractable
{
	GENERATED_BODY()

public:

	virtual FInteract_Info& Get_Interact_Info() = 0;
	virtual void Interact(AActor* actor) = 0;
};
//------------------------------------------------------------------------------------------------------------