#pragma once

#include "IN/IN.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AN_Attack_Start_End.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UAN_Attack_Start_End : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
//------------------------------------------------------------------------------------------------------------