#pragma once

#include "IN/IN.h"
#include "Blueprint/DragDropOperation.h"
#include "Drag_Drop_Operation.generated.h"
//------------------------------------------------------------------------------------------------------------
class UStorage_Component;
class UW_Base_Slot;
//------------------------------------------------------------------------------------------------------------
USTRUCT()
struct IN_API FDrag_Drop_Info
{
	GENERATED_BODY();

	UPROPERTY() int32 Index = -1;
	UPROPERTY() TObjectPtr<UStorage_Component> Storage_Component = nullptr;
	UPROPERTY() TObjectPtr<UW_Base_Slot> Widget_Slot = nullptr;
};
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDrag_Drop_Operation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY() FDrag_Drop_Info Drag_Info;
};
//------------------------------------------------------------------------------------------------------------