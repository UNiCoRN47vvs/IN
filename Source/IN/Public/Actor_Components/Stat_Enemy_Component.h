#pragma once

#include "Actor_Components/Stat_Master_Component.h"
#include "Stat_Enemy_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UStat_Enemy_Component : public UStat_Master_Component
{
	GENERATED_BODY()
	
public:
	UStat_Enemy_Component();

	virtual void Init_Stat_Component(UDA_Character_Master* da_character) override;
	virtual void Apply_Damage(float damage_value) override;
};
//------------------------------------------------------------------------------------------------------------