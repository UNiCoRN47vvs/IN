#include "Widgets/W_Item_Slot.h"
#include "Components/Border.h"
#include "Actor_Components/Storage_Component.h"
//------------------------------------------------------------------------------------------------------------
void UW_Item_Slot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Is_Can_Drop = false;

	if (!Border || Is_Dragged || !Storage_Component)
		return;

	UDA_Item_Master* da_item = nullptr;
	UDrag_Drop_Operation* drag_drop_operation = Cast<UDrag_Drop_Operation>(InOperation);
	CHECK_PTR(drag_drop_operation);
	CHECK_PTR(drag_drop_operation->Drag_Info.Storage_Component);

	const TArray<FItem>& storage = drag_drop_operation->Drag_Info.Storage_Component->Get_Storage();

	if (!storage.IsValidIndex(drag_drop_operation->Drag_Info.Index))
		return;

	CHECK_PTR(storage[drag_drop_operation->Drag_Info.Index].Item);

	da_item = storage[drag_drop_operation->Drag_Info.Index].Item->Get_Item_DA();

	CHECK_PTR(da_item);

	if (Equipment_Slot_Tags.HasTag(da_item->Item_Tag) && Storage_Component->Get_Storage().IsValidIndex(Storage_Index) && !Storage_Component->Get_Storage()[Storage_Index].Item)
	{
		Border->SetBrush(Empty_Border_Brush);
		Is_Can_Drop = true;
	}
	else
	{
		Border->SetBrush(Occupied_Border_Brush);
		return;
	}
}
//------------------------------------------------------------------------------------------------------------