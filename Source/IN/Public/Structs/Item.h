#pragma once

#include "IN/IN.h"
#include "Items/Item_Master.h"
#include "Data_Assets/DA_Item_Master.h"
#include "Widgets/W_Base_Slot.h"
#include "Item.generated.h"
//------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct IN_API FItem
{
	GENERATED_BODY()

public:
	void Item_Add(UItem_Master* item);
	void Item_Combine(UDA_Item_Master* item_da, int32& item_count);
	void Item_Destroy();

	TMulticastDelegate<void(int32, UDA_Item_Master*)> On_Update_Slot;

	UPROPERTY() TObjectPtr<UItem_Master> Item = nullptr;
protected:
	
};
//------------------------------------------------------------------------------------------------------------