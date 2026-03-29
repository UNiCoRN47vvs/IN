#include "Widgets/W_Inventory_Slot.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Actor_Components/Storage_Component.h"
#include "Widgets/W_Drop_Slot.h"
#include "Widgets/DDO/Drag_Drop_Operation.h"
//------------------------------------------------------------------------------------------------------------
void UW_Inventory_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}
//------------------------------------------------------------------------------------------------------------
void UW_Inventory_Slot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Is_Can_Drop = false;

	if (!Storage_Component)
		return;

	if (!Border || Is_Dragged)
		return;

	if (Storage_Component->Get_Storage()[Storage_Index].Item)
		Border->SetBrush(Occupied_Border_Brush);
	else
	{
		Border->SetBrush(Empty_Border_Brush);
		Is_Can_Drop = true;
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_Inventory_Slot::Init_Slot(UStorage_Component* storage_component, int32 storage_index)
{
	Super::Init_Slot(storage_component, storage_index);
}
//------------------------------------------------------------------------------------------------------------