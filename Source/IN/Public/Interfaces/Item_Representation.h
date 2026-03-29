#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item_Representation.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Master;
//------------------------------------------------------------------------------------------------------------
UINTERFACE(MinimalAPI)
class UItem_Representation : public UInterface
{
	GENERATED_BODY()
};
//------------------------------------------------------------------------------------------------------------
class IN_API IItem_Representation
{
	GENERATED_BODY()

public:

	virtual UDA_Item_Master* Get_Item_DA() const = 0;
};
//------------------------------------------------------------------------------------------------------------