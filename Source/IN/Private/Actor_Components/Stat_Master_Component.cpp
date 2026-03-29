#include "Actor_Components/Stat_Master_Component.h"
#include "Net/UnrealNetwork.h"
#include "Data_Assets/DA_Character_Master.h"
//------------------------------------------------------------------------------------------------------------
UStat_Master_Component::UStat_Master_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

	Stat_Path.Add(EStat_Name::Health_Current, &Health_Current);
	Stat_Path.Add(EStat_Name::Health_Max, &Health_Max);
	Stat_Path.Add(EStat_Name::Health_Regen, &Health_Regen);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		FTimerManager& timer_manager = GetWorld()->GetTimerManager();

		timer_manager.SetTimer(TH_Health, this, &UStat_Master_Component::Apply_Health_Timer, Period_Heath, true);
	}
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStat_Master_Component, Health_Current);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::Init_Stat_Component(UDA_Character_Master* da_character)
{
	CHECK_PTR(da_character);
	float* stat_value = nullptr;

	for (const TPair<EStat_Name, float>& stat : da_character->Stats)
	{
		if (!Stat_Path.Contains(stat.Key))
			continue;

		stat_value = *Stat_Path.Find(stat.Key);
		if(stat_value)
			*stat_value = stat.Value;
	}
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::On_Rep_Health()
{
	On_Health_Change.Broadcast(Health_Current, Health_Max);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::Apply_Health_Timer()
{
	Health_Current = FMath::Clamp(Health_Current + Health_Regen, 0.0f, Health_Max);

	if (GetNetMode() != NM_DedicatedServer)
		On_Rep_Health();
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::Update_All_Bars()
{
	On_Rep_Health();
}
//------------------------------------------------------------------------------------------------------------
void UStat_Master_Component::Apply_Damage(float damage_value)
{
	Health_Current -= damage_value;
}
//------------------------------------------------------------------------------------------------------------
