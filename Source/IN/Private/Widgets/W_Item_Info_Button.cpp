#include "Widgets/W_Item_Info_Button.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Button::Initialize_Item_Info_Button(const EInteract_Item_Type item_type, const int32 index, TMulticastDelegate<void(const EInteract_Item_Type, const int32)>* delegate, const FText& text)
{
	Item_Type = item_type;
	Index = index;
	On_Item_Interacted = delegate;

	Button_Text->SetText(text);
}
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Button::NativeConstruct()
{
	Super::NativeConstruct();

	ensureMsgf(Button, TEXT("Variable Empty"));

	Button->OnClicked.AddUniqueDynamic(this, &UW_Item_Info_Button::Button_Clicked);
}
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Button::NativeDestruct()
{
	Super::NativeDestruct();
	if(On_Item_Interacted)
		On_Item_Interacted->RemoveAll(this);
}
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Button::Button_Clicked()
{
	On_Item_Interacted->Broadcast(Item_Type, Index);
}
//------------------------------------------------------------------------------------------------------------