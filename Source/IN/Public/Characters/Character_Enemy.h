#pragma once

#include "Characters/Character_Master.h"
#include "Data_Assets/DA_Character_Enemy.h"
#include "Interfaces/Interactable.h"
#include "Character_Enemy.generated.h"
//------------------------------------------------------------------------------------------------------------
class UStat_Enemy_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API ACharacter_Enemy : public ACharacter_Master, public IInteractable
{
	GENERATED_BODY()
public:
	ACharacter_Enemy();
	virtual void Apply_Damage(float damage_value, ACharacter_Master* damage_owner) override;
	virtual FInteract_Info& Get_Interact_Info() override;
	virtual void Interact(AActor* actor) override;

	void Set_Speed_Value(EStat_Name stat_name);
	void Enemy_Attack(AActor* target_actor);
	UStat_Enemy_Component* Get_Stat_Enemy_Component();
	inline UDA_Character_Enemy* Get_DA_Character_Enemy() const { return Cast<UDA_Character_Enemy>(DA_Character); }

	TDelegate<void(AActor*)> On_Was_Attacked;
protected:
	virtual void BeginPlay() override;

	virtual void Apply_Death() override;
	virtual void Client_Apply_Death() override;

	FInteract_Info Interact_Info;
	UPROPERTY() TObjectPtr<UStat_Enemy_Component> Stat_Enemy_Component;
};
//------------------------------------------------------------------------------------------------------------