#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Bars_Box.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Bars_Box : public UUserWidget
{
	GENERATED_BODY()
	
public:
protected:
	virtual void NativeConstruct() override;
	void Init();

	UFUNCTION(BlueprintImplementableEvent)	void Init_Bars(UStat_Player_Component* stat_player_component);
};
//------------------------------------------------------------------------------------------------------------