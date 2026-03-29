#include "Widgets/W_Interact_Info.h"
#include "Components/TextBlock.h"
#include "Interfaces/Interactable.h"
//------------------------------------------------------------------------------------------------------------
void UW_Interact_Info::NativeDestruct()
{
	Super::NativeDestruct();

	if (On_Interact)
		On_Interact->RemoveAll(this);
}
//------------------------------------------------------------------------------------------------------------
void UW_Interact_Info::Init_Interact_Widget(TMulticastDelegate<void(FInteract_Info*)>* delegate)
{
	CHECK_PTR(delegate);

	On_Interact = delegate;

	On_Interact->AddUObject(this, &UW_Interact_Info::Execute_Interact);
}
//------------------------------------------------------------------------------------------------------------
void UW_Interact_Info::Execute_Interact(FInteract_Info* info)
{
	if (!info)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);

	if (Name_Action)
		Name_Action->SetText(info->Name_Action);
	if (Name_Owner)
		Name_Owner->SetText(info->Name_Owner);
	if (Name_Button)
		Name_Button->SetText(info->Name_Button);
}
//------------------------------------------------------------------------------------------------------------