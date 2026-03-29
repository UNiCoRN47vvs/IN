#pragma once

#include "IN/IN.h"
#include "GameFramework/HUD.h"
#include "Main_HUD.generated.h"
//------------------------------------------------------------------------------------------------------------
class UW_Permanent;
class UW_Base_Slot;
class UStorage_Component;
class APlayer_Controller;
class ICharacter_State;
class UW_Container;
class UDA_Interact_Widget_Class;
struct FInteract_Info;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API AMain_HUD : public AHUD
{
	GENERATED_BODY()
public:
	AMain_HUD();
	void Init_Main_HUD();
	void Init_Interact_Widget(TMulticastDelegate<void(FInteract_Info*)>* delegate);
	void Toggle_Inventory(UStorage_Component* component_player, UStorage_Component* component_interact, UDA_Interact_Widget_Class* interact_widget_class_da);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY() TScriptInterface<ICharacter_State> Character_State;
	UPROPERTY() TObjectPtr<APlayer_Controller> Player_Controller;
	UPROPERTY() TObjectPtr<UW_Container> Active_Widget;
	UPROPERTY() TObjectPtr<UW_Permanent> Permanent_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main HUD") TSubclassOf<UW_Permanent> Permanent_Widget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main HUD") TSubclassOf<UW_Container> Inventory_Widget_Class;
private:
	uint8 Is_Inventory_Open : 1;
};
//------------------------------------------------------------------------------------------------------------