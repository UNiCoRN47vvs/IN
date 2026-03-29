#include "Widgets/W_Item_Info_Menu.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Widgets/W_Item_Info_Button.h"
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Menu::Initialize_Item_Info_Menu(const TArray<EInteract_Item_Type>& types, const int32 index, TMulticastDelegate<void(const EInteract_Item_Type, const int32)>* delegate)
{
	UW_Item_Info_Button* item_info_button = nullptr;
	FText button_text = {};
	for (const EInteract_Item_Type& type : types)
	{
		item_info_button = CreateWidget<UW_Item_Info_Button>(this, W_Item_Info_Button_Class);
		if (!item_info_button)
			continue;

		item_info_button->Button->OnClicked.AddUniqueDynamic(this, &UW_Item_Info_Menu::RemoveFromParent);

		button_text = Text_Values.FindRef(type);

		Vertical_Box->AddChild(item_info_button);
		item_info_button->Initialize_Item_Info_Button(type, index, delegate, button_text);
	}

	GetWorld()->GetTimerManager().SetTimer(Hide_Timer_Handle, this, &UW_Item_Info_Menu::RemoveFromParent, 0.666f, false);
}
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Menu::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	GetWorld()->GetTimerManager().ClearTimer(Hide_Timer_Handle);
}
//------------------------------------------------------------------------------------------------------------
void UW_Item_Info_Menu::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	GetWorld()->GetTimerManager().SetTimer(Hide_Timer_Handle, this, &UW_Item_Info_Menu::RemoveFromParent, 0.666f, false);

}
//------------------------------------------------------------------------------------------------------------
