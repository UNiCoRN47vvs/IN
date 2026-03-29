#include "Actor_Components/Equipment_Component.h"
//------------------------------------------------------------------------------------------------------------
UEquipment_Component::UEquipment_Component()
{
}
//------------------------------------------------------------------------------------------------------------
void UEquipment_Component::BeginPlay()
{
	Storage_Size = Equipment_Slot_Tags.Num();

	Super::BeginPlay();

	if (!GetOwner()->HasAuthority() || GetNetMode() != NM_DedicatedServer)
	{
		Is_Init = true;
	}
}
//------------------------------------------------------------------------------------------------------------
