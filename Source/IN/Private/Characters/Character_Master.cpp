#include "Characters/Character_Master.h"
#include "Engine/OverlapResult.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IAnimationBudgetAllocator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Actor_Components/Quick_Bar_Component.h"
#include "Actor_Components/Stat_Master_Component.h"
#include "Actor_Components/Inventory_Component.h"
#include "Actor_Components/Items/Item_Comp_Master.h"
#include "Items/Item_Interactive.h"
#include "Data_Assets/DA_Character_Master.h"
#include "Data_Assets/DA_Item_Interactive.h"
#include "Enums/Team_Type.h"
//------------------------------------------------------------------------------------------------------------
ACharacter_Master::ACharacter_Master()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SetReplicateMovement(true);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	int32 count = 0;
	uint32 flag = 1;
	uint32 last_flag = static_cast<uint32>(EState_Flags::Last);

	while (flag < last_flag)
	{
		++count;
		flag <<= 1;
	}

	State_Flag_Delegates.SetNum(count);
}
//------------------------------------------------------------------------------------------------------------
UQuick_Bar_Component* ACharacter_Master::Get_Quick_Bar_Component()
{
	return Quick_Bar_Component ? Quick_Bar_Component : Quick_Bar_Component = FindComponentByClass<UQuick_Bar_Component>();
}
//------------------------------------------------------------------------------------------------------------
UInventory_Component* ACharacter_Master::Get_Inventory_Component()
{
	return Inventory_Component ? Inventory_Component : Inventory_Component = FindComponentByClass<UInventory_Component>();
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Master::Has_Flag(const EState_Flags flag) const
{
	return EnumHasAnyFlags<EState_Flags>(State_Flags, flag);
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Master::Has_All_Flags(const EState_Flags flags) const
{
	return EnumHasAllFlags<EState_Flags>(State_Flags, flags);
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Master::Is_Attack_Blocked() const
{
	return Has_All_Flags(EState_Flags::Attacking | EState_Flags::Melee)
		|| Has_Flag(EState_Flags::Dead);
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Master::Is_Move_Blocked() const
{
	return Has_All_Flags(EState_Flags::Attacking | EState_Flags::Melee)
		|| Has_Flag(EState_Flags::Dead);
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Master::Is_Inventory_Blocked() const
{
	return Has_Flag(EState_Flags::Attacking)
		|| Has_Flag(EState_Flags::Dead);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::BeginPlay()
{
	Super::BeginPlay();
	UStat_Master_Component* stat_master_component = nullptr;
	USkeletalMeshComponent* mesh = GetMesh();
	CHECK_PTR(mesh);

	if (DA_Character)
		Add_Team_Type_Tag(DA_Character->Team_Type_Self);

	if (Get_Quick_Bar_Component())
		Quick_Bar_Component->On_Hand_Item_Changed.AddUObject(this, &ACharacter_Master::On_Hand_Item_Changed);

	if (HasAuthority())
	{
		stat_master_component = FindComponentByClass<UStat_Master_Component>();
		CHECK_PTR(stat_master_component);

		stat_master_component->On_Death.AddUObject(this, &ACharacter_Master::S_Apply_Death);

		Create_Fist_Item();

		Get_Flag_Delegate(EState_Flags::Attacking).AddUObject(this, &ACharacter_Master::On_State_Attacking);
		//On_Attacking_State_Changed.AddUObject(this, &ACharacter_Master::On_Attacking_State_Changed);
	}

	IAnimationBudgetAllocator::Get(GetWorld())->SetEnabled(true); //??????????
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (TMulticastDelegate<void(bool)>& delegate : State_Flag_Delegates)
	{
		delegate.RemoveAll(this);
	}

	if (Get_Quick_Bar_Component())
	{
		Quick_Bar_Component->On_Hand_Item_Changed.RemoveAll(this);
		Quick_Bar_Component->On_Slot_Drag_Triggered.RemoveAll(this);
	}

	UStat_Master_Component* stat_master_component = FindComponentByClass<UStat_Master_Component>();
	CHECK_PTR(stat_master_component);

	stat_master_component->On_Death.RemoveAll(this);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacter_Master, State_Flags);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::S_Add_Remove_State_Flags_Implementation(EState_Flags state_flag, bool is_adding)
{
	if(is_adding)
		State_Flags |= state_flag;
	else
		State_Flags &= ~state_flag;

	Get_Flag_Delegate(state_flag).Broadcast(is_adding);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::On_State_Update()
{
	EState_Flags flag = {};
	uint32 bit_index = 0;
	uint32 changed_bits = static_cast<uint32>(State_Flags ^ Old_State_Flags);

	while (changed_bits != 0)
	{
		bit_index = FMath::CountTrailingZeros(changed_bits);

		flag = static_cast<EState_Flags>(1 << bit_index);

		if (EnumHasAnyFlags(State_Flags, flag))
			Get_Flag_Delegate(flag).Broadcast(true);
		else
			Get_Flag_Delegate(flag).Broadcast(false);

		changed_bits &= ~(1 << bit_index);
	}

	Old_State_Flags = State_Flags;
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Set_Attack_Mode(const EAttack_Mode attack_mode)
{
	switch (attack_mode)
	{
	case EAttack_Mode::None:
		CHECK_ALARM(DA_Character, TEXT("DA_Character is nullptr!"));
		CHECK_ALARM(GetCharacterMovement(), TEXT("GetCharacterMovement is nullptr!"));

		GetCharacterMovement()->MaxWalkSpeed = DA_Character->Stats.FindRef(EStat_Name::Speed_Run);

		S_Add_Remove_State_Flags(EState_Flags::Melee, false);
		S_Add_Remove_State_Flags(EState_Flags::Ranged, false);

		break;

	case EAttack_Mode::Melee:
		S_Add_Remove_State_Flags(EState_Flags::Ranged, false);
		S_Add_Remove_State_Flags(EState_Flags::Melee, true);

		break;

	case EAttack_Mode::Ranged:
		CHECK_ALARM(DA_Character, TEXT("DA_Character is nullptr!"));
		CHECK_ALARM(GetCharacterMovement(), TEXT("GetCharacterMovement is nullptr!"));

		GetCharacterMovement()->MaxWalkSpeed = DA_Character->Stats.FindRef(EStat_Name::Speed_Run) / 2.0f;

		S_Add_Remove_State_Flags(EState_Flags::Melee, false);
		S_Add_Remove_State_Flags(EState_Flags::Ranged, true);

		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Create_Fist_Item()
{
	CHECK_ALARM(DA_Fist_Item, TEXT("DA_Fist_Item is nullptr!"));

	Fist_Item = NewObject<UItem_Interactive>(this, DA_Fist_Item->Item_Object_Class);
	CHECK_ALARM(Fist_Item, TEXT("Fist item is nullptr!"));

	Fist_Item->Init_Item(1, DA_Fist_Item, 47); // 47 is arbitrary ID for fist

	On_Hand_Item_Changed(DA_Fist_Item, Fist_Item, 47);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::On_Hand_Item_Changed(UDA_Item_Master* da_item_master, UItem_Interactive* item_interactive, int32 index)
{
	UDA_Item_Master* item_da = da_item_master;
	UItem_Interactive* item = item_interactive;

	if (Current_Hand_Component)
	{
		if (Current_Hand_Component->Remove_And_Check_Same_Item(item_interactive)) // if same item, remove item
		{
			item_da = DA_Fist_Item;
			item = Fist_Item;
		} else if (item_da && Current_Hand_Component->GetClass() == item_da->Item_Comp_Class)
		{
			Current_Hand_Component->Init_Item_Comp(item_da, GetMesh(), item);
			return;
		}

		Current_Hand_Component->Deactivate();
		Current_Hand_Component->DestroyComponent();
		Current_Hand_Component = nullptr;
	}

	CHECK_ALARM(item_da, TEXT("item_da is nullptr!"));
	CHECK_ALARM(item_da->Item_Comp_Class, TEXT("Item_Comp_Class is nullptr!"));

	Current_Hand_Component = NewObject<UItem_Comp_Master>(this, item_da->Item_Comp_Class);

	CHECK_ALARM(Current_Hand_Component, TEXT("component nullptr!"));

	Current_Hand_Component->RegisterComponent();
	Current_Hand_Component->Init_Item_Comp(item_da, GetMesh(), item);

	AddInstanceComponent(Current_Hand_Component);

	Current_Hand_Component->On_Attacking.BindUObject(this, &ACharacter_Master::On_Hand_Comp_Attacking);
	Current_Hand_Component->On_Apply_Damage.BindUObject(this, &ACharacter_Master::S_Apply_Damage);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::On_Hand_Comp_Attacking(bool is_attacking)
{
	S_Add_Remove_State_Flags(EState_Flags::Attacking, is_attacking);

	//On_Attacking.Broadcast(is_attacking);

	if (!is_attacking)
		Set_Attack_Mode(EAttack_Mode::None);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::On_State_Attacking(bool is_attacking)
{
	//if(is_attacking)
		//S_Attack();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::S_Attack_Implementation()
{
	if (Is_Attack_Blocked())
		return;

	CHECK_ALARM(Current_Hand_Component, TEXT("Current_Hand_Component is nullptr!"));

	Set_Attack_Mode(Current_Hand_Component->Get_Attack_Mode());

	S_Add_Remove_State_Flags(EState_Flags::Attacking, true);
	/*Is_Attacking = true;

	On_Attacking.Broadcast(Is_Attacking);*/

	C_Attack_Rotate();

	NM_Play_Attack_Montage();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::NM_Play_Attack_Montage_Implementation()
{
	if (Current_Hand_Component)
		PlayAnimMontage(Current_Hand_Component->Get_Montage_Interactive());
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::C_Attack_Rotate_Implementation()
{
	Attack_Rotate();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Attack_Rotate()
{
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::S_Apply_Damage_Implementation(const float damage_value, AActor* damaged_actor)
{
	if (Has_Flag(EState_Flags::Dead))
		return;

	IDamage* damaged = nullptr;

	damaged = Cast<IDamage>(damaged_actor);

	CHECK_PTR(damaged);

	damaged->Apply_Damage(damage_value, this);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Apply_Damage(float damage_value, ACharacter_Master* damage_owner)
{
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::S_Apply_Death_Implementation()
{
	Apply_Death();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Apply_Death()
{
	S_Add_Remove_State_Flags(EState_Flags::Dead, true);
	/*Is_Dead = true;

	On_Death.Broadcast();*/

	NM_Apply_Death();

	Deactivate_Components();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::NM_Apply_Death_Implementation()
{
	Client_Apply_Death();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Client_Apply_Death()
{
	Deactivate_Components();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Deactivate_Components()
{
	UStat_Master_Component* stat_master_component = nullptr;

	stat_master_component = FindComponentByClass<UStat_Master_Component>();

	CHECK_ALARM(stat_master_component, TEXT("Component = nullptr!!!"));
	stat_master_component->Deactivate();

	CHECK_ALARM(GetCharacterMovement(), TEXT("Component = nullptr!!!"));
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	CHECK_ALARM(GetCapsuleComponent(), TEXT("Component = nullptr!!!"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Dead_Enemy"));
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Toggle_Inventory(UDA_Interact_Widget_Class* interact_widget_class_da, UStorage_Component* interact_component)
{
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::C_Toggle_Inventory_Implementation(UDA_Interact_Widget_Class* interact_widget_class_da, UStorage_Component* interact_component)
{
	Toggle_Inventory(interact_widget_class_da, interact_component);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Add_Team_Type_Tag(ETeam_Type team_type)
{
	const UEnum* enumPtr = StaticEnum<ETeam_Type>();
	FName team_type_name = {};

	enumPtr ? team_type_name = enumPtr->GetNameByValue(static_cast<int64>(team_type)) : team_type_name = TEXT("Unknown");

	if (!Tags.Contains(team_type_name))
		Tags.Add(team_type_name);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Master::Remove_Team_Type_Tag(ETeam_Type team_type)
{
	const UEnum* enumPtr = StaticEnum<ETeam_Type>();

	Tags.RemoveSingle(enumPtr ? enumPtr->GetNameByValue(static_cast<int64>(team_type)) : TEXT("Unknown"));
}
//------------------------------------------------------------------------------------------------------------
