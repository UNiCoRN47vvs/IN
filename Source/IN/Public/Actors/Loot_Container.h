#pragma once

#include "IN/IN.h"
#include "GameFramework/Actor.h"
#include	"Interfaces/Interactable.h"
#include "Loot_Container.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDrop_Component;
class UStorage_Component;
class UDA_Loot_Container;
struct FDrop_Item;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API ALoot_Container : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ALoot_Container();
	virtual FInteract_Info& Get_Interact_Info() override;
	virtual void Interact(AActor* actor) override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void On_Init_Drop(const TArray<FDrop_Item>& items);
	void Open_Container_W(AActor* actor);

	FInteract_Info Interact_Info;
	FGuid Loot_Container_ID;
	FDateTime Last_Opened_Time;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot_Container") TObjectPtr<UStorage_Component> Storage_Component;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot_Container") TObjectPtr<UDrop_Component> Drop_Component;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot_Container") TObjectPtr<UDA_Loot_Container> DA_Loot_Container;
};
//------------------------------------------------------------------------------------------------------------