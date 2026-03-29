#include "Widgets/W_Base_Slot.h"
#include "InputCoreTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Controller/Player_Controller.h"
#include "Actor_Components/Storage_Component.h"
#include "Data_Assets/DA_Item_Master.h"
#include "Widgets/W_Drop_Slot.h"
#include "Widgets/DDO/Drag_Drop_Operation.h"
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	Is_Dragged = false;

	if (Border)
		Base_Border_Brush = Border->Background;
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::NativeDestruct()
{
	Super::NativeDestruct();
	On_Item_Info_Menu_Create.Clear();
	On_Quick_Drop.Unbind();

	if (On_Update_Slot)
		On_Update_Slot->RemoveAll(this);
}
//------------------------------------------------------------------------------------------------------------
FReply UW_Base_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const bool is_left_mouse = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	const bool is_left_control = InMouseEvent.IsLeftControlDown();

	if (is_left_control && is_left_mouse && Storage_Component && Storage_Component->Get_Storage().IsValidIndex(Storage_Index) && Storage_Component->Get_Storage()[Storage_Index].Item)
	{
		On_Quick_Drop.ExecuteIfBound(Storage_Index);
		return FReply::Handled();
	}

	if (is_left_mouse)
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

	return FReply::Handled();
}
//------------------------------------------------------------------------------------------------------------
FReply UW_Base_Slot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && Storage_Component)
	{
		On_Item_Info_Menu_Create.Broadcast(Storage_Index);
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!Storage_Component || !Storage_Component->Get_Storage().IsValidIndex(Storage_Index) || !Storage_Component->Get_Storage()[Storage_Index].Item)
		return;

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDrag_Drop_Operation* drag_drop_operation = nullptr;

	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDrag_Drop_Operation::StaticClass());

	OutOperation->Payload = this;
	OutOperation->Pivot = EDragPivot::CenterCenter;
	OutOperation->Offset = FVector2D{ 0.0f };

	drag_drop_operation = Cast<UDrag_Drop_Operation>(OutOperation);
	CHECK_PTR(drag_drop_operation);

	CHECK_PTR(drag_drop_operation);
	drag_drop_operation->DefaultDragVisual = Create_Drag_Drop_Widget();

	drag_drop_operation->Drag_Info.Index = Storage_Index;
	drag_drop_operation->Drag_Info.Storage_Component = Storage_Component;
	drag_drop_operation->Drag_Info.Widget_Slot = this;

	Is_Dragged = true;

	if (!Border)
		return;

	Border->SetBrush(Dragged_Border_Brush);
}
//------------------------------------------------------------------------------------------------------------
bool UW_Base_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!Is_Can_Drop)
		return false;

	if (!InOperation || !Storage_Component || !Storage_Component->Get_Storage().IsValidIndex(Storage_Index))
	{
		ALARM_LOG;
		return false;
	}

	UItem_Master* dropped_item = nullptr;
	UDrag_Drop_Operation* drag_drop_operation = nullptr;
	APlayer_Controller* controller = nullptr;

	drag_drop_operation = Cast<UDrag_Drop_Operation>(InOperation);
	controller = GetOwningPlayer<APlayer_Controller>();
	if (!drag_drop_operation || !controller)
	{
		ALARM_LOG;
		return false;
	}

	dropped_item = Storage_Component->Get_Storage()[Storage_Index].Item;

	if (!dropped_item)
		controller->S_Drop_Triggered(Storage_Component, drag_drop_operation->Drag_Info, Storage_Index);
	else
	{
		if (!drag_drop_operation->Drag_Info.Storage_Component
			|| !drag_drop_operation->Drag_Info.Storage_Component->Get_Storage().IsValidIndex(drag_drop_operation->Drag_Info.Index)
			|| !drag_drop_operation->Drag_Info.Storage_Component->Get_Storage()[drag_drop_operation->Drag_Info.Index].Item)
		{
			ALARM_LOG;
			return false;
		}

		if (dropped_item->Get_Item_DA() == drag_drop_operation->Drag_Info.Storage_Component->Get_Storage()[drag_drop_operation->Drag_Info.Index].Item->Get_Item_DA())
			controller->S_Merge_Triggered(Storage_Component, drag_drop_operation->Drag_Info, Storage_Index);
	}

	Clear_Triggered_State();
	return false;
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UDrag_Drop_Operation* drag_drop_operation = Cast<UDrag_Drop_Operation>(InOperation);
	CHECK_PTR(drag_drop_operation);

	drag_drop_operation->Drag_Info.Widget_Slot->Clear_Triggered_State(true);

	Is_Dragged = false;
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Clear_Triggered_State();
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::Init_Slot(UStorage_Component* storage_component, int32 storage_index)
{
	CHECK_PTR(storage_component);

	Storage_Component = storage_component;
	Storage_Index = storage_index;

	On_Update_Slot = Storage_Component->Get_Item_Slot_Delegate(Storage_Index);
	if (On_Update_Slot)
		On_Update_Slot->AddUObject(this, &UW_Base_Slot::Update_Slot);

	const FItem& item = Storage_Component->Get_Storage()[storage_index];

	if (item.Item)
		Update_Slot(item.Item->Item_Count, item.Item->Get_Item_DA());
	else
		Update_Slot(0, nullptr);
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::Update_Slot(int32 item_count, UDA_Item_Master* item_da)
{
	if (Item_Count_TB)
	{
		if (item_count <= 1)
			Item_Count_TB->SetVisibility(ESlateVisibility::Collapsed);
		else
		{
			Item_Count_TB->SetVisibility(ESlateVisibility::Visible);
			Item_Count_TB->SetText(FText::AsNumber(item_count));
		}
	}

	if (!Item_Icon_I)
		return;

	if (!item_da)
		Item_Icon_I->SetBrushFromTexture(Base_Icon_Texture);
	else
		Item_Icon_I->SetBrushFromSoftTexture(item_da->Item_Icon);
}
//------------------------------------------------------------------------------------------------------------
void UW_Base_Slot::Clear_Triggered_State(bool is_drag_end)
{
	if (Border && (!Is_Dragged || is_drag_end))
		Border->SetBrush(Base_Border_Brush);
}
//------------------------------------------------------------------------------------------------------------
UW_Drop_Slot* UW_Base_Slot::Create_Drag_Drop_Widget()
{
	UW_Drop_Slot* drop_widget = CreateWidget<UW_Drop_Slot>(this, W_Drop_Slot_Class);
	int32 row_size = 0;
	int32 column_size = 0;
	FSlateBrush brush = Item_Icon_I->GetBrush();

	if (!drop_widget)
		return nullptr;

	drop_widget->Init_Drop_Slot_Widget(brush);

	return drop_widget;
}
//------------------------------------------------------------------------------------------------------------