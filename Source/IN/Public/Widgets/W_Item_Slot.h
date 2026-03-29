#pragma once

#include "Widgets/W_Base_Slot.h"
#include "GameplayTagContainer.h"
#include "W_Item_Slot.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Item_Slot : public UW_Base_Slot
{
	GENERATED_BODY()
public:

protected:
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Item_Slot") FGameplayTagContainer Equipment_Slot_Tags;
};
//------------------------------------------------------------------------------------------------------------