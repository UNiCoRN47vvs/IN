#include "Anim_Notify/AN_Attack_Start_End.h"
#include "Actor_Components/Items/Item_Comp_Master.h"
//------------------------------------------------------------------------------------------------------------
void UAN_Attack_Start_End::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APawn* owner_pawn = MeshComp->GetOwner<APawn>();
	CHECK_ALARM(owner_pawn, TEXT("Owner is not a pawn!"));

	UItem_Comp_Master* item_comp_master = owner_pawn->FindComponentByClass<UItem_Comp_Master>();
	CHECK_PTR(item_comp_master);

	if(owner_pawn->IsLocallyControlled())
		item_comp_master->Notify_Attack(true);
}
//------------------------------------------------------------------------------------------------------------
void UAN_Attack_Start_End::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APawn* owner_pawn = MeshComp->GetOwner<APawn>();
	CHECK_ALARM(owner_pawn, TEXT("Owner is not a pawn!"));

	UItem_Comp_Master* item_comp_master = owner_pawn->FindComponentByClass<UItem_Comp_Master>();
	CHECK_PTR(item_comp_master);

	if (owner_pawn->IsLocallyControlled())
		item_comp_master->Notify_Attack(false);
}
//------------------------------------------------------------------------------------------------------------