#include "Actor_Components/Stat_Enemy_Component.h"
#include "Data_Assets/DA_Character_Enemy.h"
//------------------------------------------------------------------------------------------------------------
UStat_Enemy_Component::UStat_Enemy_Component()
{
	SetIsReplicatedByDefault(true);
}
//------------------------------------------------------------------------------------------------------------
void UStat_Enemy_Component::Init_Stat_Component(UDA_Character_Master* da_character)
{
	Super::Init_Stat_Component(da_character);
	
}
//------------------------------------------------------------------------------------------------------------
void UStat_Enemy_Component::Apply_Damage(float damage_value)
{
	Super::Apply_Damage(damage_value);

	if (Health_Current <= 0.0f)
	{
		On_Death.Broadcast();
		// TODO :: Apply Death!

		//Temp!!!
		//GetOwner()->Destroy();
	}
}
//------------------------------------------------------------------------------------------------------------
