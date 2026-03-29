#pragma once

#include "Widgets/W_Item_Slot.h"
#include "W_Equipment_Slot.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Equipment_Slot : public UW_Item_Slot
{
	GENERATED_BODY()
	
public:
	virtual void Init_Slot(UStorage_Component* storage_component, int32 storage_index) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Equipment_Slot") int32 Equipment_Slot_Index;
};
//------------------------------------------------------------------------------------------------------------