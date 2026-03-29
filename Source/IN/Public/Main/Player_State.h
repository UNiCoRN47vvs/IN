#pragma once

#include "IN/IN.h"
#include "GameFramework/PlayerState.h"
#include "Player_State.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API APlayer_State : public APlayerState
{
	GENERATED_BODY()
public:
	APlayer_State();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
//------------------------------------------------------------------------------------------------------------