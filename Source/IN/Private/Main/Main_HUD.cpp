#include "Main/Main_HUD.h"
#include "Controller/Player_Controller.h"
#include "Interfaces/Character_State.h"
#include "Widgets/W_Permanent.h"
#include "Widgets/W_Container.h"
#include "Widgets/W_Base_Slot.h"
#include "Data_Assets/DA_Interact_Widget_Class.h"
#include "Structs/Item.h"
//------------------------------------------------------------------------------------------------------------
AMain_HUD::AMain_HUD()
{
	Is_Inventory_Open = false;
}
//------------------------------------------------------------------------------------------------------------
void AMain_HUD::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
void AMain_HUD::Init_Main_HUD()
{
	CHECK_ALARM(Permanent_Widget_Class, TEXT("Class Variable Empty"));

	Permanent_Widget = CreateWidget<UW_Permanent>(PlayerOwner, Permanent_Widget_Class);
	CHECK_PTR(Permanent_Widget);
	Permanent_Widget->AddToViewport();

	CHECK_CAST(Player_Controller, APlayer_Controller, PlayerOwner);

	Player_Controller->HUD_Is_Ready(this);
}
//------------------------------------------------------------------------------------------------------------
void AMain_HUD::Init_Interact_Widget(TMulticastDelegate<void(FInteract_Info*)>* delegate)
{
	CHECK_PTR(delegate);
	CHECK_PTR(Permanent_Widget);

	Permanent_Widget->Init_Interact_Widget(delegate);
}
//------------------------------------------------------------------------------------------------------------
void AMain_HUD::Toggle_Inventory(UStorage_Component* component_player, UStorage_Component* component_interact, UDA_Interact_Widget_Class* interact_widget_class_da)
{
	if (!Character_State.GetInterface())
	{
		ensureMsgf(PlayerOwner, TEXT("nullptr!"));

		ICharacter_State* character_state = Cast<ICharacter_State>(PlayerOwner->GetPawn());
		ensureMsgf(character_state, TEXT("nullptr!"));

		Character_State.SetInterface(character_state);
		Character_State.SetObject(PlayerOwner);
	}

	if (Character_State->Is_Inventory_Blocked())
		return;

	if (Is_Inventory_Open)
	{
		Active_Widget->Close_Container_Widget();
		Active_Widget->RemoveFromParent();
		Is_Inventory_Open = false;
		return;
	}

	UW_Container* w_storage = nullptr;
	TSubclassOf<UW_Container> widget_class = component_interact && interact_widget_class_da && interact_widget_class_da->Interact_Widget_Class ? interact_widget_class_da->Interact_Widget_Class : Inventory_Widget_Class;

	w_storage = CreateWidget<UW_Container>(PlayerOwner, widget_class);
	CHECK_PTR(w_storage);

	w_storage->AddToViewport(1);
	w_storage->Init_Container_Widget(component_player, component_interact);

	Active_Widget = w_storage;
	Is_Inventory_Open = true;
}
//------------------------------------------------------------------------------------------------------------

