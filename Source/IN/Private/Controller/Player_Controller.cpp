#include "Controller/Player_Controller.h"
#include "Net/UnrealNetwork.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Main/Main_HUD.h"
#include "Characters/Character_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Actor_Components/Storage_Component.h"
//------------------------------------------------------------------------------------------------------------
APlayer_Controller::APlayer_Controller()
{
	Is_Inventory_Open = false;
	Is_HUD_Init = false;
	Is_Aim = false;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* subsystem = nullptr;
	ACharacter_Player* character_player = nullptr;
	UEnhancedInputComponent* enhanced_input_component = nullptr;
	USpringArmComponent* spring_arm_component = nullptr;

	SetInputMode(FInputModeGameOnly{});
	SetControlRotation(FRotator(Min_Pitch / 2.0f, 0.0f, 0.0f));

	if (HasAuthority())
	{
		Possess(GetPawn());
	}

	subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	CHECK_PTR(subsystem);
	CHECK_PTR(MC_Default);

	subsystem->AddMappingContext(MC_Default, 0);

	if (subsystem->GetUserSettings())
		subsystem->GetUserSettings()->RegisterInputMappingContext(MC_Default);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = Min_Pitch; // Ограничение низа
		PlayerCameraManager->ViewPitchMax = Max_Pitch; // Ограничение верха
	}

	enhanced_input_component = Cast<UEnhancedInputComponent>(InputComponent);
	if (enhanced_input_component)
	{
		character_player = GetPawn<ACharacter_Player>();

		if (character_player)
		{
			spring_arm_component = character_player->FindComponentByClass<USpringArmComponent>();
			if (spring_arm_component)
				spring_arm_component->TargetArmLength = (Min_Camera_Distance + Max_Camera_Distance) / 2.0f;
			else
				CHECK_ALARM(spring_arm_component, TEXT("NULLPTR!!!"));

			//character_player->On_Apply_Death.AddUObject(this, &AController_Main_Game::ApplyMax_Camera_Distance_Death);
			//character_player->On_Dodging.AddUObject(this, &AController_Main_Game::Dodging);
			character_player->On_Toggle_Inventory.AddUObject(this, &APlayer_Controller::Toggle_Inventory);


			enhanced_input_component->BindAction(Action_Crouch, ETriggerEvent::Started, character_player, &ACharacter_Player::Player_Crouch);
			enhanced_input_component->BindAction(Action_Walk, ETriggerEvent::Started, character_player, &ACharacter_Player::S_Player_Walk);
			enhanced_input_component->BindAction(Action_Sprint, ETriggerEvent::Started, character_player, &ACharacter_Player::S_Player_Sprint);
			enhanced_input_component->BindAction(Action_Inventory, ETriggerEvent::Started, character_player, &ACharacter_Player::Toggle_Inventory);
			enhanced_input_component->BindAction(Action_Interact, ETriggerEvent::Started, character_player, &ACharacter_Player::Player_Interact);
			enhanced_input_component->BindAction(Action_LMB, ETriggerEvent::Started, character_player, &ACharacter_Player::LMB_Click);
			enhanced_input_component->BindAction(Action_RMB, ETriggerEvent::Triggered, character_player, &ACharacter_Player::S_RMB_Triggered);
			enhanced_input_component->BindAction(Action_RMB, ETriggerEvent::Completed, character_player, &ACharacter_Player::S_RMB_Released);
			enhanced_input_component->BindAction(Action_RMB, ETriggerEvent::Canceled, character_player, &ACharacter_Player::S_RMB_Released);

			Character_Player = character_player;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HasAuthority())
	{

	}

	if (GetNetMode() != NM_DedicatedServer)
	{
		if (Is_HUD_Init)
			return;

		AMain_HUD* main_hud = GetHUD<AMain_HUD>();
		CHECK_PTR(main_hud);

		main_hud->Init_Main_HUD();
	}

}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (Is_HUD_Init)
		return;

	AMain_HUD* main_hud = GetHUD<AMain_HUD>();
	CHECK_PTR(main_hud);

	main_hud->Init_Main_HUD();
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::HUD_Is_Ready(AMain_HUD* main_hud)
{
	CHECK_PTR(main_hud);
	UEnhancedInputComponent* enhanced_input_component = Cast<UEnhancedInputComponent>(InputComponent);
	ACharacter_Player* player_character = GetPawn<ACharacter_Player>();
	CHECK_PTR(enhanced_input_component);
	CHECK_PTR(player_character);

	//enhanced_input_component->BindAction(Action_Inventory, ETriggerEvent::Started, main_hud, &AMain_HUD::Toggle_Inventory);
	player_character->On_Toggle_Inventory_HUD.AddUObject(main_hud, &AMain_HUD::Toggle_Inventory);

	main_hud->Init_Interact_Widget(&player_character->On_Update_Interact_Widget);

	Is_HUD_Init = true;

}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhanced_input_component = Cast<UEnhancedInputComponent>(InputComponent);
	CHECK_PTR(enhanced_input_component);

	enhanced_input_component->BindAction(Action_Look, ETriggerEvent::Triggered, this, &APlayer_Controller::Input_Look);
	enhanced_input_component->BindAction(Action_Move, ETriggerEvent::Triggered, this, &APlayer_Controller::Input_Move);
	enhanced_input_component->BindAction(Action_Jump, ETriggerEvent::Started, this, &APlayer_Controller::Input_Jump_Started);
	enhanced_input_component->BindAction(Action_Zoom, ETriggerEvent::Triggered, this, &APlayer_Controller::Input_Zoom);

	enhanced_input_component->BindAction(Action_Quick_Slot_1, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<1>);
	enhanced_input_component->BindAction(Action_Quick_Slot_2, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<2>);
	enhanced_input_component->BindAction(Action_Quick_Slot_3, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<3>);
	enhanced_input_component->BindAction(Action_Quick_Slot_4, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<4>);
	enhanced_input_component->BindAction(Action_Quick_Slot_5, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<5>);
	enhanced_input_component->BindAction(Action_Quick_Slot_6, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<6>);
	enhanced_input_component->BindAction(Action_Quick_Slot_7, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<7>);
	enhanced_input_component->BindAction(Action_Quick_Slot_8, ETriggerEvent::Started, this, &APlayer_Controller::Trigger_Quick_Slot<8>);

	enhanced_input_component->BindAction(Action_RMB, ETriggerEvent::Triggered, this, &APlayer_Controller::Aim_Triggered);
	enhanced_input_component->BindAction(Action_RMB, ETriggerEvent::Canceled, this, &APlayer_Controller::Aim_Released);
	enhanced_input_component->BindAction(Action_RMB, ETriggerEvent::Completed, this, &APlayer_Controller::Aim_Released);

}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(APlayer_Controller, Is_Attacking);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Input_Move(const FInputActionValue& value)
{
	CHECK_CAST(Character_Player, ACharacter_Player, GetPawn());

	if (Is_Inventory_Open || Character_Player->Is_Move_Blocked())
		return;

	APawn* pawn = GetPawn();
	CHECK_PTR(pawn);

	const FVector2D movement_vector = value.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yaw_rotation(0, rotation.Yaw, 0); // Игнорируем наклон вверх/вниз (Pitch), чтобы не входить в землю
	const FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X); // Получаем направление "Вперед" относительно поворота камеры
	const FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y); // Получаем направление "Вправо" относительно поворота камеры

	pawn->AddMovementInput(forward_direction, movement_vector.Y);
	pawn->AddMovementInput(right_direction, movement_vector.X);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Input_Look(const FInputActionValue& value)
{
	if (Is_Inventory_Open)
		return;

	FVector2D look_axis_vector = value.Get<FVector2D>();

	AddYawInput(look_axis_vector.X);
	AddPitchInput(-look_axis_vector.Y);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Input_Jump_Started()
{
	CHECK_CAST(Character_Player, ACharacter_Player, GetPawn());

	if (Is_Inventory_Open || Character_Player->Is_Move_Blocked())
		return;

	Character_Player->Jump();
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Input_Zoom(const FInputActionValue& value)
{
	if (Is_Aim)
		return;

	APawn* controlled_pawn = GetPawn();
	CHECK_PTR(controlled_pawn);

	USpringArmComponent* camera_boom = nullptr;
	float current_length = 0.0f;
	float new_length = 0.0f;
	const float scroll_value = value.Get<float>();
	if (scroll_value == 0.0f)
		return;

	camera_boom = controlled_pawn->FindComponentByClass<USpringArmComponent>();
	if (camera_boom)
	{
		current_length = camera_boom->TargetArmLength;
		new_length = current_length - (scroll_value * Zoom_Speed);

		new_length = FMath::Clamp(new_length, Min_Camera_Distance, Max_Camera_Distance);

		camera_boom->TargetArmLength = new_length;
	}
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Aim_Triggered()
{
	if (Aim_Distance_Current <= Aim_Camera_Distance + 7.5f && Aim_Distance_Current >= Aim_Camera_Distance - 7.5f)
		return;

	APawn* controlled_pawn = GetPawn();
	CHECK_PTR(controlled_pawn);

	USpringArmComponent* spring_arm_comp = controlled_pawn->FindComponentByClass<USpringArmComponent>();
	CHECK_ALARM(spring_arm_comp, TEXT("spring_arm_comp = nullptr!"));

	if (!Is_Aim)
	{
		Is_Aim = true;
		Aim_Distance_Old = spring_arm_comp->TargetArmLength;
	}

	Aim_Distance_Current = FMath::FInterpTo(spring_arm_comp->TargetArmLength, Aim_Camera_Distance, GetWorld()->GetDeltaSeconds(), Aim_Interp_Speed);
	spring_arm_comp->TargetArmLength = Aim_Distance_Current;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Aim_Released()
{
	Is_Aim = false;

	Move_Camera_Back();
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Move_Camera_Back()
{
	if (Is_Aim || Aim_Distance_Current <= Aim_Distance_Old + 7.5f && Aim_Distance_Current >= Aim_Distance_Old - 7.5f)
		return;

	APawn* controlled_pawn = GetPawn();
	CHECK_PTR(controlled_pawn);

	USpringArmComponent* spring_arm_comp = controlled_pawn->FindComponentByClass<USpringArmComponent>();
	CHECK_ALARM(spring_arm_comp, TEXT("spring_arm_comp = nullptr!"));

	Aim_Distance_Current = FMath::FInterpTo(spring_arm_comp->TargetArmLength, Aim_Distance_Old, GetWorld()->GetDeltaSeconds(), Aim_Interp_Speed);
	spring_arm_comp->TargetArmLength = Aim_Distance_Current;


	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayer_Controller::Move_Camera_Back);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Toggle_Inventory()
{
	CHECK_CAST(Character_Player, ACharacter_Player, GetPawn());

	if (Character_Player->Is_Inventory_Blocked())
		return;

	if (Is_Inventory_Open)
	{
		SetInputMode(FInputModeGameOnly{});
	}
	else
	{
		SetInputMode(FInputModeGameAndUI{});
	}

	Is_Inventory_Open = !Is_Inventory_Open;
	bShowMouseCursor = Is_Inventory_Open;
}
//------------------------------------------------------------------------------------------------------------
template<int32 number> inline void APlayer_Controller::Trigger_Quick_Slot()
{
	CHECK_CAST(Character_Player, ACharacter_Player, GetPawn());

	Character_Player->S_Quick_Slot_Item_Use(number);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::S_Merge_Triggered_Implementation(UStorage_Component* storage_component, FDrag_Drop_Info drag_info, const int32 drop_index)
{
	CHECK_ALARM(storage_component, TEXT("component = nullptr"));
	storage_component->Merge_Triggered(drag_info, drop_index);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::S_Drop_Triggered_Implementation(UStorage_Component* storage_component, FDrag_Drop_Info drag_info, const int32 drop_index)
{
	CHECK_ALARM(storage_component, TEXT("component = nullptr"));
	storage_component->Drop_Triggered(drag_info, drop_index);
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::S_Quick_Drop_Implementation(const int32 index, UStorage_Component* owner_component, UStorage_Component* target_component)
{
	if(target_component)
		target_component->Quick_Drop_Item(index, owner_component);
}
//------------------------------------------------------------------------------------------------------------