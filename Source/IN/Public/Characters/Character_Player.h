#pragma once

#include "Characters/Character_Master.h"
#include "Character_Player.generated.h"
//------------------------------------------------------------------------------------------------------------
class UStat_Player_Component;
class IInteractable;
class UEquipment_Component;
class UW_Container;
class UStorage_Component;
class UInventory_Component;
struct FInteract_Info;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API ACharacter_Player : public ACharacter_Master
{
	GENERATED_BODY()
	friend class APlayer_Controller;

public:
	ACharacter_Player();

	virtual bool Is_Attack_Blocked() const override;
	virtual bool Is_Move_Blocked() const override;
	virtual bool Is_Inventory_Blocked() const override;
	virtual void Apply_Damage(float damage_value, ACharacter_Master* damage_owner) override;
	virtual void Toggle_Inventory(UDA_Interact_Widget_Class* interact_widget_class_da, UStorage_Component* interact_component = nullptr) override;
	virtual void Set_Attack_Mode(const EAttack_Mode attack_mode) override;

	UStat_Player_Component* Get_Stat_Player_Component();
	UEquipment_Component* Get_Equipment_Component();

	TMulticastDelegate<void(FInteract_Info*)> On_Update_Interact_Widget;
	TMulticastDelegate<void()> On_Toggle_Inventory;
	TMulticastDelegate<void(UStorage_Component*, UStorage_Component*, UDA_Interact_Widget_Class*)> On_Toggle_Inventory_HUD;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Attack_Rotate() override;

	void Player_Crouch();
	void Player_Interact();
	void Spawn_Interactable_Box();
	void LMB_Click();
	void Toggle_Inventory();

#pragma region On_State_Changed_Functions
	void On_State_Couch(bool is_couch);
	void On_State_Walk_Server(bool is_sprinting);
	void On_State_Walk_Client(bool is_sprinting);
	void On_State_Sprint_Server(bool is_sprinting);
	void On_State_Sprint_Client(bool is_sprinting);
#pragma endregion

	UFUNCTION(Server, Unreliable) void S_RMB_Triggered();
	UFUNCTION(Server, Unreliable) void S_RMB_Released();
	UFUNCTION(Server, Unreliable) void S_Player_Walk();
	UFUNCTION(Server, Unreliable) void S_Player_Sprint();
	UFUNCTION(Server, Unreliable) void S_Toggle_Inventory();
	UFUNCTION(Server, Reliable) void S_Player_Interact(const TScriptInterface<IInteractable>& interactable_object);

	UFUNCTION(Server, Reliable) void S_Quick_Slot_Item_Use(int32 number);

	FTimerHandle Sprint_Handle;

	UPROPERTY() TObjectPtr<UCharacterMovementComponent> Movement_Component;
	UPROPERTY() TObjectPtr<UStat_Player_Component> Stat_Player_Component;
	UPROPERTY() TObjectPtr<UEquipment_Component> Equipment_Component;
	UPROPERTY() TScriptInterface<IInteractable> Interactable_Object;
private:
	//uint8 Is_Walking : 1;
	//uint8 Is_Sprinting : 1;
	//uint8 Is_Inventory_Open : 1;
	FTimerHandle Interactable_Box_Spawn_Handle;
	FVector Interactable_Box_Size;
};
//------------------------------------------------------------------------------------------------------------