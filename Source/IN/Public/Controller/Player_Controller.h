#pragma once

#include "IN/IN.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Widgets/DDO/Drag_Drop_Operation.h"
#include "Player_Controller.generated.h"
//------------------------------------------------------------------------------------------------------------
class UInputMappingContext;
class UInputAction;
class ACharacter_Player;
class UStorage_Component;
class AMain_HUD;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()
public:
	APlayer_Controller();
	void HUD_Is_Ready(AMain_HUD* main_hud);

	UFUNCTION(Server, Reliable) void S_Drop_Triggered(UStorage_Component* storage_component, FDrag_Drop_Info drag_info, const int32 drop_index);
	UFUNCTION(Server, Reliable) void S_Merge_Triggered(UStorage_Component* storage_component, FDrag_Drop_Info drag_info, const int32 drop_index);
	UFUNCTION(Server, Reliable) void S_Quick_Drop(const int32 index, UStorage_Component* owner_component, UStorage_Component* target_component);
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Input_Move(const FInputActionValue& value);
	void Input_Look(const FInputActionValue& value);
	void Input_Jump_Started();
	void Input_Zoom(const FInputActionValue& value);
	void Aim_Triggered();
	void Aim_Released();
	void Toggle_Inventory();
	void Move_Camera_Back();
	template <int32 number>	void Trigger_Quick_Slot();

	UPROPERTY() TObjectPtr<ACharacter_Player> Character_Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputMappingContext> MC_Default;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Jump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Look;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Zoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Crouch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Walk;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Sprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Inventory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Interact;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_7;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_Quick_Slot_8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_LMB;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input") TObjectPtr<UInputAction> Action_RMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Min_Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Max_Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Min_Camera_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Max_Camera_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Aim_Camera_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Zoom_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Controller") float Aim_Interp_Speed;	

private:
	uint8 Is_HUD_Init : 1;
	uint8 Is_Inventory_Open : 1;
	uint8 Is_Aim : 1;
	float Aim_Distance_Old;
	float Aim_Distance_Current;
};
//------------------------------------------------------------------------------------------------------------
