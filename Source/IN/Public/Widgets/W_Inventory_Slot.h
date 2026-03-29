#pragma once

#include "Widgets/W_Base_Slot.h"
#include "W_Inventory_Slot.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Inventory_Slot : public UW_Base_Slot
{
	GENERATED_BODY()
	
public:
	virtual void Init_Slot(UStorage_Component* storage_component, int32 storage_index) override;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
//------------------------------------------------------------------------------------------------------------