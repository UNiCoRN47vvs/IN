#include "Widgets/W_Quick_Slot_Bar.h"
#include "Components/HorizontalBox.h"
#include "Actor_Components/Quick_Bar_Component.h"
#include "Widgets/W_Quick_Slot.h"
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot_Bar::NativeConstruct()
{
	Super::NativeConstruct();
	
	Init();
}
//------------------------------------------------------------------------------------------------------------
void UW_Quick_Slot_Bar::Init()
{
	UW_Quick_Slot* quick_slot = nullptr;

	CHECK_PTR(GetOwningPlayerPawn());
	Quick_Bar_Component = GetOwningPlayerPawn()->FindComponentByClass<UQuick_Bar_Component>();

	if (!Quick_Bar_Component || !Quick_Bar_Component->Is_Component_Init())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UW_Quick_Slot_Bar::Init);
		return;
	}


	for (int32 i = 0; i < HB_Bar->GetAllChildren().Num(); i++)
	{
		quick_slot = Cast<UW_Quick_Slot>(HB_Bar->GetAllChildren()[i]);
		if (!quick_slot)
		{
			ensureMsgf(quick_slot, TEXT("quick_slot is nullptr!"));
			continue;
		}

		quick_slot->Init_Slot(Quick_Bar_Component, i);
	}
}
//------------------------------------------------------------------------------------------------------------
