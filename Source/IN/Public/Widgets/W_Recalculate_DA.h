#pragma once

#include "IN/IN.h"
#include "EditorUtilityWidget.h"
#include "W_Recalculate_DA.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract, meta = (ShowWorldContextPin), config = Editor)
class IN_API UW_Recalculate_DA : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable) void On_Clicked_Button_Recalculate_DA();
};
//------------------------------------------------------------------------------------------------------------