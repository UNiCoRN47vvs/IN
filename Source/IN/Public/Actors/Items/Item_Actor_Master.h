#pragma once

#include "IN/IN.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Item_Representation.h"
#include "Item_Actor_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Master;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API AItem_Actor_Master : public AActor, public IItem_Representation, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AItem_Actor_Master();

#pragma region Interface Representation
	inline virtual UDA_Item_Master* Get_Item_DA() const override { return Item_DA; }
#pragma endregion
#pragma region Interface Interactable
	virtual FInteract_Info& Get_Interact_Info() override;
	virtual void Interact(AActor* actor) override;
#pragma endregion

	UFUNCTION(Server, Reliable) void Init_Item(UDA_Item_Master* item_da, int32 item_count);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Actor_Master", Replicated) int32 Item_Count;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FInteract_Info Interact_Info;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Actor_Master", Replicated) TObjectPtr<UDA_Item_Master> Item_DA;
};
//------------------------------------------------------------------------------------------------------------