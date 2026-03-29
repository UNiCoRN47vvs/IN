#pragma once

#include "Items/Item_Master.h"
#include "Item_Consumable.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UItem_Consumable : public UItem_Master
{
	GENERATED_BODY()
public:
	virtual void Item_Use() override;

};
//------------------------------------------------------------------------------------------------------------