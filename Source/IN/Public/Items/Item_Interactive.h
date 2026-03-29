#pragma once

#include "Items/Item_Master.h"
#include "Item_Interactive.generated.h"
//------------------------------------------------------------------------------------------------------------
class UQuick_Bar_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UItem_Interactive : public UItem_Master
{
	GENERATED_BODY()

public:
	virtual void Item_Use() override;
protected:
	UQuick_Bar_Component* Get_Quick_Bar_Component();

	UPROPERTY() TObjectPtr<UQuick_Bar_Component> Quick_Bar_Component;
};
//------------------------------------------------------------------------------------------------------------