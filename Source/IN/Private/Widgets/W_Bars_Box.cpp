#include "Widgets/W_Bars_Box.h"
#include "Actor_Components/Stat_Player_Component.h"
//------------------------------------------------------------------------------------------------------------
void UW_Bars_Box::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}
//------------------------------------------------------------------------------------------------------------
void UW_Bars_Box::Init()
{
	UStat_Player_Component* stat_component = nullptr;

	CHECK_PTR(GetOwningPlayerPawn());
	stat_component = GetOwningPlayerPawn()->FindComponentByClass<UStat_Player_Component>();

	if (!stat_component || !stat_component->Is_Component_Init())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UW_Bars_Box::Init);
		return;
	}

	Init_Bars(stat_component);
}
//------------------------------------------------------------------------------------------------------------
