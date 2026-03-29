#pragma once

#include "IN/IN.h"
#include "GameFramework/Character.h"
#include "Interfaces/Character_State.h"
#include "Interfaces/Damage.h"
#include "Enums/Attack_Mode.h"
#include "Enums/State_Flags.h"
#include "Character_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
class USphereComponent;
class UStat_Master_Component;
class UQuick_Bar_Component;
class UDA_Character_Master;
class UItem_Interactive;
class UDA_Item_Interactive;
class UDA_Item_Master;
class UStorage_Component;
class UInventory_Component;
class UDA_Interact_Widget_Class;
class UItem_Comp_Master;
enum class ETeam_Type : uint8;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API ACharacter_Master : public ACharacter, public ICharacter_State, public IDamage
{
	GENERATED_BODY()

public:
	ACharacter_Master();

	virtual bool Is_Attack_Blocked() const override;
	virtual bool Is_Move_Blocked() const override;
	virtual bool Is_Inventory_Blocked() const override;
	virtual void Apply_Damage(float damage_value, ACharacter_Master* damage_owner) override;
	virtual void Toggle_Inventory(UDA_Interact_Widget_Class* interact_widget_class_da, UStorage_Component* interact_component = nullptr);
	virtual void Set_Attack_Mode(const EAttack_Mode attack_mode);

	UQuick_Bar_Component* Get_Quick_Bar_Component();
	UInventory_Component* Get_Inventory_Component();

	bool Has_Flag(const EState_Flags flag) const;
	bool Has_All_Flags(const EState_Flags flags) const;

	void Add_Team_Type_Tag(ETeam_Type team_type);
	void Remove_Team_Type_Tag(ETeam_Type team_type);
	UFUNCTION(Client, Unreliable) void C_Toggle_Inventory(UDA_Interact_Widget_Class* interact_widget_class_da, UStorage_Component* interact_component);
	UFUNCTION(Server, Reliable) void S_Add_Remove_State_Flags(EState_Flags state_flag, bool is_adding);

	//TMulticastDelegate<void(bool)> On_Attacking;
	//TMulticastDelegate<void()> On_Death;

#pragma region Character_State
	inline TMulticastDelegate<void(bool)>& Get_Flag_Delegate(const EState_Flags flag) { return State_Flag_Delegates[static_cast<int32>(FMath::FloorLog2(static_cast<uint32>(flag)))]; };
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Apply_Death();
	virtual void Client_Apply_Death();
	virtual void Attack_Rotate();

	void On_Hand_Item_Changed(UDA_Item_Master* da_item_master, UItem_Interactive* item_interactive, int32 index);
	void On_Hand_Comp_Attacking(bool is_attacking);
	void On_State_Attacking(bool is_attacking);

	void Deactivate_Components();

	UFUNCTION(Server, Reliable) void S_Attack();
	UFUNCTION(Client, Reliable) void C_Attack_Rotate();
	UFUNCTION(NetMulticast, Reliable) void NM_Play_Attack_Montage();
	UFUNCTION(Server, Reliable) void S_Apply_Death();
	UFUNCTION(NetMulticast, Reliable) void NM_Apply_Death();

	TArray<TMulticastDelegate<void(bool)>> State_Flag_Delegates;
	UPROPERTY(Replicated, ReplicatedUsing="On_State_Update") EState_Flags State_Flags;

	UPROPERTY() TObjectPtr<UQuick_Bar_Component> Quick_Bar_Component;
	UPROPERTY() TObjectPtr<UInventory_Component> Inventory_Component;
	UPROPERTY(Replicated) TObjectPtr<UItem_Comp_Master> Current_Hand_Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") TObjectPtr<UDA_Character_Master> DA_Character;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") TObjectPtr<UDA_Item_Interactive> DA_Fist_Item;
private:
	void Create_Fist_Item();

	UFUNCTION() void On_State_Update();
	UFUNCTION(Server, Reliable) void S_Apply_Damage(const float damage_value, AActor* damaged_actor);

	EState_Flags Old_State_Flags;

	UPROPERTY() TObjectPtr<UItem_Interactive> Fist_Item;
};
//------------------------------------------------------------------------------------------------------------
