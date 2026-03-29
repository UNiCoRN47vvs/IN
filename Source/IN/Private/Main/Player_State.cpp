#include "Main/Player_State.h"
#include "Net/UnrealNetwork.h"
//------------------------------------------------------------------------------------------------------------
APlayer_State::APlayer_State()
{
	bReplicates = true;
	
}
//------------------------------------------------------------------------------------------------------------
void APlayer_State::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
void APlayer_State::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
