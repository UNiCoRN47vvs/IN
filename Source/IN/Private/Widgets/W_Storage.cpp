#include "Widgets/W_Storage.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Widgets/W_Inventory_Slot.h"
#include "Widgets/W_Item_Info_Menu.h"
#include "Widgets/W_Equipment.h"
#include "Actor_Components/Storage_Component.h"
#include "Controller/Player_Controller.h"
//------------------------------------------------------------------------------------------------------------
void UW_Storage::NativeDestruct()
{
	Super::NativeDestruct();
}
//------------------------------------------------------------------------------------------------------------
void UW_Storage::Init_Storage_Widget(UStorage_Component* component, UStorage_Component* target_drop_component)
{
	CHECK_PTR(component);
	CHECK_ALARM(Uniform_Grid_Panel, TEXT("Variable Empty"));
	CHECK_ALARM(W_Inventory_Slot_Class, TEXT("Class Variable Empty"));

	int32 row = 0;
	int32 column = 0;
	UW_Inventory_Slot* inventory_slot = nullptr;
	Storage_Component = component;
	Storage_Component->Is_Widget_Open = true;
	Target_Drop_Component = target_drop_component;

	const TArray<FItem>& storage = Storage_Component->Get_Storage();

	for (int32 i = 0; i < storage.Num(); i++)
	{
		inventory_slot = CreateWidget<UW_Inventory_Slot>(this, W_Inventory_Slot_Class);
		if (!inventory_slot)
			continue;

		inventory_slot->On_Item_Info_Menu_Create.AddUObject(this, &UW_Storage::Item_Info_Menu_Create);
		inventory_slot->On_Quick_Drop.BindUObject(this, &UW_Storage::On_Quick_Drop);

		row = i / Row_Size;
		column = i % Row_Size;

		Uniform_Grid_Panel->AddChildToUniformGrid(inventory_slot, row, column);

		inventory_slot->Init_Slot(Storage_Component, i);
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_Storage::Close_Widget()
{
	if(Storage_Component)
		Storage_Component->Is_Widget_Open = false;
}
//------------------------------------------------------------------------------------------------------------
void UW_Storage::Item_Info_Menu_Create(const int32 index)
{
	CHECK_PTR(Storage_Component);

	float mouse_x = 0.0f;
	float mouse_y = 0.0f;
	float viewport_scale = 0.0f;
	const float edge_padding = 5.0f;

	FVector2D viewport_size = {};
	FVector2D widget_size = {};
	const TArray<FItem>& storage = Storage_Component->Get_Storage();

	if (W_Item_Info_Menu)
		W_Item_Info_Menu->RemoveFromParent();

	if (!storage.IsValidIndex(index) || !storage[index].Item || !storage[index].Item->Get_Item_DA())
		return;

	const TArray<EInteract_Item_Type>& interact_types = storage[index].Item->Get_Item_DA()->Item_Interact_Types;

	if (interact_types.IsEmpty())
	{
		ALARM_LOG;
		return;
	}

	W_Item_Info_Menu = CreateWidget<UW_Item_Info_Menu>(this, W_Item_Info_Menu_Class);
	CHECK_PTR(W_Item_Info_Menu);

	W_Item_Info_Menu->AddToViewport(10);
	W_Item_Info_Menu->Initialize_Item_Info_Menu(interact_types, index, &Storage_Component->On_Item_Interacted);
	W_Item_Info_Menu->ForceLayoutPrepass();

	GetOwningPlayer()->GetMousePosition(mouse_x, mouse_y);

	if (GEngine && GEngine->GameViewport) // Получение размера экрана в пикселях
		GEngine->GameViewport->GetViewportSize(viewport_size);

	// Получение размера виджета и учитывание DPI Scale, GetDesiredSize возвращает размер в Slate Units (не в пикселях).
	// Нужно перевести это в пиксели, чтобы сравнивать с мышью и экраном.
	viewport_scale = UWidgetLayoutLibrary::GetViewportScale(this);
	widget_size = W_Item_Info_Menu->GetDesiredSize() * viewport_scale;

	if (mouse_x + widget_size.X > viewport_size.X) // Проверка по горизонтали (Правый край)
		mouse_x = viewport_size.X - widget_size.X - edge_padding;

	if (mouse_y + widget_size.Y > viewport_size.Y) // Проверка по вертикали (Нижний край)
		mouse_y = viewport_size.Y - widget_size.Y - edge_padding;

	mouse_x = FMath::Max(mouse_x, edge_padding);
	mouse_y = FMath::Max(mouse_y, edge_padding);

	W_Item_Info_Menu->SetPositionInViewport(FVector2D(mouse_x, mouse_y));
}
//------------------------------------------------------------------------------------------------------------
void UW_Storage::On_Quick_Drop(const int32 index)
{
	CHECK_ALARM(Storage_Component, TEXT("Variable Empty"));
	APlayer_Controller* controller = Cast<APlayer_Controller>(GetOwningPlayer());
	CHECK_PTR(controller);

	controller->S_Quick_Drop(index, Storage_Component, Target_Drop_Component);
}
//------------------------------------------------------------------------------------------------------------