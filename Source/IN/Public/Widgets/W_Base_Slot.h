#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Base_Slot.generated.h"
//------------------------------------------------------------------------------------------------------------
class UBorder;
class UImage;
class UTextBlock;
class UStorage_Component;
class UDA_Item_Master;
class UW_Drop_Slot;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Base_Slot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void Init_Slot(UStorage_Component* storage_component, int32 storage_index);

	void Clear_Triggered_State(bool is_drag_end = false);
	TDelegate<void(const int32)> On_Quick_Drop;
	TMulticastDelegate<void(const int32)> On_Item_Info_Menu_Create;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UW_Drop_Slot* Create_Drag_Drop_Widget();
	void Update_Slot(int32 item_count, UDA_Item_Master* item_da);

	uint8 Is_Can_Drop : 1;
	uint8 Is_Dragged : 1;
	int32 Storage_Index;

	TMulticastDelegate<void(int32, UDA_Item_Master*)>* On_Update_Slot;

	FSlateBrush Base_Border_Brush;
	UPROPERTY() TObjectPtr<UStorage_Component> Storage_Component;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base_Slot", meta = (BindWidget)) TObjectPtr<UTextBlock> Item_Count_TB;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base_Slot", meta = (BindWidget)) TObjectPtr<UImage> Item_Icon_I;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base_Slot", meta = (BindWidget)) TObjectPtr<UBorder> Border;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Slot") TObjectPtr<UTexture2D> Base_Icon_Texture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Slot") FSlateBrush Dragged_Border_Brush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Slot") FSlateBrush Empty_Border_Brush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Slot") FSlateBrush Occupied_Border_Brush;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base_Slot") TSubclassOf<UW_Drop_Slot> W_Drop_Slot_Class;

};
//------------------------------------------------------------------------------------------------------------