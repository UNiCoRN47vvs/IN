#include "Characters/Character_Player.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Actor_Components/Stat_Player_Component.h"
#include "Actor_Components/Equipment_Component.h"
#include "Actor_Components/Quick_Bar_Component.h"
#include "Actor_Components/Inventory_Component.h"
#include "Interfaces/Interactable.h"
#include "Data_Assets/DA_Character_Player.h"
#include "Data_Assets/DA_Item_Interactive.h"
//------------------------------------------------------------------------------------------------------------
ACharacter_Player::ACharacter_Player()
{
	Interactable_Box_Size = FVector(50.0f, 90.0f, 90.0f);
}
//------------------------------------------------------------------------------------------------------------
UStat_Player_Component* ACharacter_Player::Get_Stat_Player_Component()
{
	return Stat_Player_Component ? Stat_Player_Component : Stat_Player_Component = FindComponentByClass<UStat_Player_Component>();
}
//------------------------------------------------------------------------------------------------------------
UEquipment_Component* ACharacter_Player::Get_Equipment_Component()
{
	return Equipment_Component ? Equipment_Component : Equipment_Component = FindComponentByClass<UEquipment_Component>();
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Player::Is_Attack_Blocked() const
{
	return Super::Is_Attack_Blocked() || Has_All_Flags(EState_Flags::Inventory_Open);
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Player::Is_Move_Blocked() const
{
	return Super::Is_Move_Blocked() || Has_All_Flags(EState_Flags::Inventory_Open);
}
//------------------------------------------------------------------------------------------------------------
bool ACharacter_Player::Is_Inventory_Blocked() const
{
	return Super::Is_Inventory_Blocked();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		Get_Flag_Delegate(EState_Flags::Walking).AddUObject(this, &ACharacter_Player::On_State_Walk_Server);
		Get_Flag_Delegate(EState_Flags::Sprinting).AddUObject(this, &ACharacter_Player::On_State_Sprint_Server);

		if (GetNetMode() != NM_DedicatedServer)
		{
			Get_Flag_Delegate(EState_Flags::Crouched).AddUObject(this, &ACharacter_Player::On_State_Couch);

			GetWorld()->GetTimerManager().SetTimer(Interactable_Box_Spawn_Handle, this, &ACharacter_Player::Spawn_Interactable_Box, 0.175f, true, 1.0f);
		}

	}
	else
	{
		if (IsLocallyControlled())
			GetWorld()->GetTimerManager().SetTimer(Interactable_Box_Spawn_Handle, this, &ACharacter_Player::Spawn_Interactable_Box, 0.175f, true, 1.0f);
			
		Get_Flag_Delegate(EState_Flags::Crouched).AddUObject(this, &ACharacter_Player::On_State_Couch);
		Get_Flag_Delegate(EState_Flags::Walking).AddUObject(this, &ACharacter_Player::On_State_Walk_Client);
		Get_Flag_Delegate(EState_Flags::Sprinting).AddUObject(this, &ACharacter_Player::On_State_Sprint_Client);
	}

	if (Get_Stat_Player_Component())
		Stat_Player_Component->Init_Stat_Component(DA_Character);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Set_Attack_Mode(const EAttack_Mode attack_mode)
{
	switch (attack_mode)
	{
	case EAttack_Mode::None:
		break;

	case EAttack_Mode::Melee:
		break;

	case EAttack_Mode::Ranged:
		if (Has_Flag(EState_Flags::Sprinting))
			S_Add_Remove_State_Flags(EState_Flags::Sprinting, false);
		else if (Has_Flag(EState_Flags::Crouched))
			S_Add_Remove_State_Flags(EState_Flags::Crouched, false);
		else if(Has_Flag(EState_Flags::Walking))
			S_Add_Remove_State_Flags(EState_Flags::Walking, false);

		break;
	}

	Super::Set_Attack_Mode(attack_mode);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Player_Crouch()
{
	if (Is_Move_Blocked() || Has_Flag(EState_Flags::Ranged))
		return;

	S_Add_Remove_State_Flags(EState_Flags::Crouched, !bIsCrouched);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::On_State_Couch(bool is_couch)
{
	if (is_couch)
		Crouch();
	else
		UnCrouch();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_Player_Walk_Implementation()
{
	if (Has_Flag(EState_Flags::Ranged))
		return;

	S_Add_Remove_State_Flags(EState_Flags::Walking, !Has_Flag(EState_Flags::Walking));
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::On_State_Walk_Server(bool is_sprinting)
{
	CHECK_CAST(Movement_Component, UCharacterMovementComponent, GetMovementComponent());

	CHECK_ALARM(DA_Character, TEXT("DA = nullptr!"));

	if (Has_Flag(EState_Flags::Sprinting))
		S_Add_Remove_State_Flags(EState_Flags::Sprinting, false);

	if (Has_Flag(EState_Flags::Crouched))
		S_Add_Remove_State_Flags(EState_Flags::Crouched, false);

	Movement_Component->MaxWalkSpeed = DA_Character->Stats.FindRef(Has_Flag(EState_Flags::Walking) ? EStat_Name::Speed_Run : EStat_Name::Speed_Walk);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::On_State_Walk_Client(bool is_sprinting)
{
	CHECK_CAST(Movement_Component, UCharacterMovementComponent, GetMovementComponent());

	CHECK_ALARM(DA_Character, TEXT("DA = nullptr!"));

	Movement_Component->MaxWalkSpeed = DA_Character->Stats.FindRef(Has_Flag(EState_Flags::Walking) ? EStat_Name::Speed_Run : EStat_Name::Speed_Walk);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_Player_Sprint_Implementation()
{
	if (Has_Flag(EState_Flags::Ranged))
		return;

	bool is_sprinting = Has_Flag(EState_Flags::Sprinting);

	S_Add_Remove_State_Flags(EState_Flags::Sprinting, !is_sprinting);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::On_State_Sprint_Server(bool is_sprinting)
{
	CHECK_CAST(Movement_Component, UCharacterMovementComponent, GetMovementComponent());

	CHECK_ALARM(DA_Character, TEXT("DA = nullptr!"));

	if (Has_Flag(EState_Flags::Walking))
		S_Add_Remove_State_Flags(EState_Flags::Walking, false);

	if (Has_Flag(EState_Flags::Crouched) && !is_sprinting)
		S_Add_Remove_State_Flags(EState_Flags::Crouched, false);

	Movement_Component->MaxWalkSpeed = DA_Character->Stats.FindRef(is_sprinting ? EStat_Name::Speed_Sprint : EStat_Name::Speed_Run);

	if (Get_Stat_Player_Component())
		Stat_Player_Component->Is_Sprinting = is_sprinting;

	if (is_sprinting && !GetWorld()->GetTimerManager().TimerExists(Sprint_Handle))
	{
		TWeakObjectPtr<ACharacter_Player> weak_this = this;
		GetWorld()->GetTimerManager().SetTimer(Sprint_Handle, [weak_this]()
			{
				if (weak_this.IsValid())
				{
					if (weak_this->Has_Flag(EState_Flags::Crouched) || weak_this->GetCharacterMovement() && weak_this->GetCharacterMovement()->Velocity.IsNearlyZero())
					{
						weak_this->S_Player_Sprint();
						return;
					}

					bool is_stamina_emty = false;
					CHECK_PTR(weak_this->Get_Stat_Player_Component());
					is_stamina_emty = weak_this->Stat_Player_Component->Sprint();

					if (is_stamina_emty)
						weak_this->S_Player_Sprint();
				}
			}, 0.25f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Sprint_Handle);
	}
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::On_State_Sprint_Client(bool is_sprinting)
{
	CHECK_CAST(Movement_Component, UCharacterMovementComponent, GetMovementComponent());

	CHECK_ALARM(DA_Character, TEXT("DA = nullptr!"));

	Movement_Component->MaxWalkSpeed = DA_Character->Stats.FindRef(is_sprinting ? EStat_Name::Speed_Sprint : EStat_Name::Speed_Run);

	if (Get_Stat_Player_Component())
		Stat_Player_Component->Is_Sprinting = is_sprinting;
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Player_Interact()
{
	if (!Interactable_Object)
		return;

	S_Player_Interact(Interactable_Object);

	Interactable_Object = nullptr;

	//TODO :: проиграть анимацию

	On_Update_Interact_Widget.Broadcast(nullptr);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_Player_Interact_Implementation(const TScriptInterface<IInteractable>& interactable_object)
{
	CHECK_PTR(interactable_object);

	interactable_object->Interact(this);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Spawn_Interactable_Box()
{
	bool is_hit = false;
	UWorld* world = GetWorld();
	AActor* target_actor = nullptr;
	FHitResult hit_result = {};
	FHitResult line_trace_hit_result = {};
	FCollisionQueryParams line_trace_params = {};
	const FVector owner_location = GetActorLocation();
	const FVector box_location = owner_location + (GetActorForwardVector() * 80.0f);
	FVector target_location = {};
	CHECK_PTR(world);
	TArray<TEnumAsByte<EObjectTypeQuery>> object_types = {};
	TArray<AActor*> ignore_actors = {};
	ignore_actors.Add(this);

	object_types.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	object_types.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel5));

	is_hit = UKismetSystemLibrary::BoxTraceSingleForObjects(this, box_location, box_location, FVector{70.0f}, GetActorRotation(),
		object_types, true, ignore_actors, EDrawDebugTrace::None, hit_result, true, FLinearColor::Red, FLinearColor::Green, 0.33f);

	if (!is_hit)
	{
		if (Interactable_Object)
		{
			On_Update_Interact_Widget.Broadcast(nullptr);
			Interactable_Object = nullptr;
		}

		return;
	}

	target_actor = hit_result.GetActor();
	target_location = target_actor->GetActorLocation();

	line_trace_params.bTraceComplex = true;
	line_trace_params.AddIgnoredActor(this);
	line_trace_params.AddIgnoredActor(target_actor);

	is_hit = world->LineTraceSingleByChannel(line_trace_hit_result, owner_location, target_location, ECC_Visibility, line_trace_params);
	//DrawDebugLine(GetWorld(), owner_location, target_location, FColor::Blue, false, 0.5f); //DEBUG

	if (is_hit)
	{
		is_hit = world->LineTraceSingleByChannel(line_trace_hit_result, owner_location + FVector{ 0.0f, 0.0f, 75.0f }, target_location, ECC_Visibility, line_trace_params);
		//DrawDebugLine(GetWorld(), owner_location + FVector{ 0.0f, 0.0f, 75.0f }, target_location, FColor::Blue, false, 0.5f); //DEBUG

		if (is_hit)
		{
			is_hit = world->LineTraceSingleByChannel(line_trace_hit_result, owner_location - FVector{ 0.0f, 0.0f, 70.0f }, target_location, ECC_Visibility, line_trace_params);
			//DrawDebugLine(GetWorld(), owner_location - FVector{ 0.0f, 0.0f, 70.0f }, target_location, FColor::Blue, false, 0.5f); //DEBUG

			if (is_hit)
			{
				if (Interactable_Object)
				{
					On_Update_Interact_Widget.Broadcast(nullptr);
					Interactable_Object = nullptr;
				}

				return;
			}
		}
	}

	Interactable_Object.SetObject(target_actor);
	Interactable_Object.SetInterface(Cast<IInteractable>(target_actor));
	CHECK_PTR(Interactable_Object);

	On_Update_Interact_Widget.Broadcast(&Interactable_Object->Get_Interact_Info());
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::LMB_Click()
{
	if (Is_Attack_Blocked())
		return;

	//TODO :: Split melee and range logic

	if (Has_Flag(EState_Flags::Crouched))
		S_Add_Remove_State_Flags(EState_Flags::Crouched, false);

	S_Attack();
	//S_Add_Remove_State_Flags(EState_Flags::Attacking, true);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Attack_Rotate()
{
	TWeakObjectPtr<ACharacter_Player> weak_this = this;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	//???????? kostil ????????
	GetWorld()->GetTimerManager().SetTimerForNextTick([weak_this]()
		{
			CHECK_WEAK_PTR(weak_this);

			UCameraComponent* camera_component = weak_this->FindComponentByClass<UCameraComponent>();
			CHECK_ALARM(camera_component, TEXT("Camera = nullptr!!"));

			weak_this->Controller->SetControlRotation(camera_component->GetForwardVector().Rotation());

			weak_this->bUseControllerRotationYaw = false;
			weak_this->GetCharacterMovement()->bOrientRotationToMovement = true;
		});
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_RMB_Released_Implementation()
{
	Set_Attack_Mode(EAttack_Mode::None);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_RMB_Triggered_Implementation()
{
	if (Has_Flag(EState_Flags::Ranged))
		return;

	Set_Attack_Mode(EAttack_Mode::Ranged);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Toggle_Inventory()
{
	Toggle_Inventory(nullptr);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Toggle_Inventory(UDA_Interact_Widget_Class* interact_widget_class_da, UStorage_Component* interact_component)
{
	if (Has_Flag(EState_Flags::Attacking))
		return;

	if (HasAuthority() && GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		C_Toggle_Inventory(interact_widget_class_da, interact_component);
		return;
	}

	S_Toggle_Inventory();

	if (Has_Flag(EState_Flags::Inventory_Open))
		State_Flags &= ~EState_Flags::Inventory_Open;
	else
		State_Flags |= EState_Flags::Inventory_Open;

	On_Toggle_Inventory.Broadcast();
	On_Toggle_Inventory_HUD.Broadcast(Get_Inventory_Component(), interact_component, interact_widget_class_da);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_Toggle_Inventory_Implementation()
{
	if (Has_Flag(EState_Flags::Attacking))
		return;

	if (Has_Flag(EState_Flags::Sprinting))
		S_Add_Remove_State_Flags(EState_Flags::Sprinting, false);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::S_Quick_Slot_Item_Use_Implementation(int32 number)
{
	if (Has_Flag(EState_Flags::Attacking))
		return;

	CHECK_PTR(Get_Quick_Bar_Component());

	Quick_Bar_Component->Quick_Slot_Item_Use(number);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Player::Apply_Damage(float damage_value, ACharacter_Master* damage_owner)
{
	Super::Apply_Damage(damage_value, damage_owner);

	CHECK_PTR(Get_Stat_Player_Component());

	Stat_Player_Component->Apply_Damage(damage_value);

}
//------------------------------------------------------------------------------------------------------------

