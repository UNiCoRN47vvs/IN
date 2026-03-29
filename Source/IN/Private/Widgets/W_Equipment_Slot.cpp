#include "Widgets/W_Equipment_Slot.h"
#include "Actor_Components/Equipment_Component.h"
//------------------------------------------------------------------------------------------------------------
void UW_Equipment_Slot::Init_Slot(UStorage_Component* storage_component, int32 storage_index)
{
	Storage_Index = Equipment_Slot_Index;

	Super::Init_Slot(storage_component, Storage_Index);
}
//------------------------------------------------------------------------------------------------------------
