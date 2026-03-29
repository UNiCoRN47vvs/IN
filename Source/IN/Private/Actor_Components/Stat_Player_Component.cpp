#include "Actor_Components/Stat_Player_Component.h"
#include "Net/UnrealNetwork.h"
#include "Data_Assets/DA_Character_Player.h"
//------------------------------------------------------------------------------------------------------------
UStat_Player_Component::UStat_Player_Component()
{
	SetIsReplicatedByDefault(true);

	Period_Heath = 5.0f;
	Period_Stamina = 0.75f;
	Period_Hunger = 17.0f;
	Period_Thirst = 15.0f;

	Is_Init = false;
	Is_Sprinting = false;

	Stat_Path.Add(EStat_Name::Stamina_Current, &Stamina_Current);
	Stat_Path.Add(EStat_Name::Stamina_Max, &Stamina_Max);
	Stat_Path.Add(EStat_Name::Stamina_Regen, &Stamina_Regen);
	Stat_Path.Add(EStat_Name::Hunger_Current, &Hunger_Current);
	Stat_Path.Add(EStat_Name::Hunger_Max, &Hunger_Max);
	Stat_Path.Add(EStat_Name::Hunger_Waste, &Hunger_Waste);
	Stat_Path.Add(EStat_Name::Thirst_Current, &Thirst_Current);
	Stat_Path.Add(EStat_Name::Thirst_Max, &Thirst_Max);
	Stat_Path.Add(EStat_Name::Thirst_Waste, &Thirst_Waste);

}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		FTimerManager& timer_manager = GetWorld()->GetTimerManager();

		if (GetNetMode() != NM_DedicatedServer) //Проверка на то если не отдельный сервер (в основном для вызова апдейта виджетов в соло так как OnRep не сработает)
		{
			On_Rep_Health();
			On_Rep_Stamina();
			On_Rep_Hunger();
			On_Rep_Thirst();

			Is_Init = true;
		}

		timer_manager.SetTimer(TH_Stamina, this, &UStat_Player_Component::Apply_Stamina_Timer, Period_Stamina, true);
		timer_manager.SetTimer(TH_Hunger, this, &UStat_Player_Component::Apply_Hunger_Timer, Period_Hunger, true);
		timer_manager.SetTimer(TH_Thirst, this, &UStat_Player_Component::Apply_Thirst_Timer, Period_Thirst, true);
	}
	else
	{
		Is_Init = true;
	}
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStat_Player_Component, Stamina_Current);
	DOREPLIFETIME(UStat_Player_Component, Hunger_Current);
	DOREPLIFETIME(UStat_Player_Component, Thirst_Current);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Init_Stat_Component(UDA_Character_Master* da_character)
{
	Super::Init_Stat_Component(da_character);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Update_All_Bars()
{
	Super::Update_All_Bars();

	On_Rep_Stamina();
	On_Rep_Hunger();
	On_Rep_Thirst();
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Apply_Damage(float damage_value)
{
	Super::Apply_Damage(damage_value);

	if (Health_Current <= 0.0f)
	{
		// TODO :: Apply Death!
		On_Death.Broadcast();
	}

	if (GetNetMode() != NM_DedicatedServer)
		On_Rep_Health();
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::On_Rep_Stamina()
{
	On_Stamina_Change.Broadcast(Stamina_Current, Stamina_Max);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::On_Rep_Hunger()
{
	On_Hunger_Change.Broadcast(Hunger_Current, Hunger_Max);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::On_Rep_Thirst()
{
	On_Thirst_Change.Broadcast(Thirst_Current, Thirst_Max);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Apply_Stamina_Timer()
{
	if (Is_Sprinting)
		return;

	Stamina_Current = FMath::Clamp(Stamina_Current + Stamina_Regen, 0.0f, Stamina_Max);

	if (GetNetMode() != NM_DedicatedServer)
		On_Rep_Stamina();
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Apply_Hunger_Timer()
{
	Hunger_Current = FMath::Clamp(Hunger_Current - Hunger_Waste, -Hunger_Max, Hunger_Max);

	if (GetNetMode() != NM_DedicatedServer)
		On_Rep_Hunger();
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Apply_Thirst_Timer()
{
	Thirst_Current = FMath::Clamp(Thirst_Current - Thirst_Waste, -Thirst_Max, Thirst_Max);

	if (GetNetMode() != NM_DedicatedServer)
		On_Rep_Thirst();
}
//------------------------------------------------------------------------------------------------------------
bool UStat_Player_Component::Sprint()
{
	Stamina_Current = FMath::Clamp(Stamina_Current - 1.5f, 0.0f, Stamina_Max);

	if (GetNetMode() != NM_DedicatedServer)
		On_Rep_Stamina();

	if (Stamina_Current <= SMALL_NUMBER)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
void UStat_Player_Component::Add_Stat_Value(const EStat_Name stat_name, const float value)
{
	float* stat_value = Stat_Path.FindRef(stat_name);
	float new_value = 0.0f;
	CHECK_PTR(stat_value);

	switch (stat_name)
	{
	case EStat_Name::Health_Current:
		new_value = FMath::Clamp(*stat_value + value, 0.0f, Health_Max);
		break;
	case EStat_Name::Stamina_Current:
		new_value = FMath::Clamp(*stat_value + value, 0.0f, Stamina_Max);
		break;
	case EStat_Name::Hunger_Current:
		new_value = FMath::Clamp(*stat_value + value, 0.0f, Hunger_Max);
		break;
	case EStat_Name::Thirst_Current:
		new_value = FMath::Clamp(*stat_value + value, 0.0f, Thirst_Max);
		break;
	}

	*stat_value = new_value;

	if (GetNetMode() != NM_DedicatedServer)
		Update_All_Bars();
}
//------------------------------------------------------------------------------------------------------------
