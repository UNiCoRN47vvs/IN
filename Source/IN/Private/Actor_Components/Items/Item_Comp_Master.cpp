#include "Actor_Components/Items/Item_Comp_Master.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Data_Assets/DA_Item_Interactive.h"
//------------------------------------------------------------------------------------------------------------
UItem_Comp_Master::UItem_Comp_Master()
{
	PrimaryComponentTick.bCanEverTick = true;
	Is_Attacking = false;
	Skip_Attack_Frame = false;

	SetIsReplicatedByDefault(true);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItem_Comp_Master, Current_Hand_Comp);
	DOREPLIFETIME(UItem_Comp_Master, Hit_Actors);
	DOREPLIFETIME(UItem_Comp_Master, Montage_Interactive);
	DOREPLIFETIME(UItem_Comp_Master, Current_Bounds);
	DOREPLIFETIME(UItem_Comp_Master, Item_DA);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	On_Attacking.Unbind();
	On_Apply_Damage.Unbind();
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::Init_Item_Comp(UDA_Item_Master* item_da, USkeletalMeshComponent* owner_mesh, UItem_Master* item)
{
	Item_DA = item_da;
	Current_Item = item;

	Is_Attacking = false;
	Can_Damaged = false;

	if (owner_mesh && Item_DA)
		owner_mesh->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &UItem_Comp_Master::On_Attack_Montage_Ended);

	UDA_Item_Interactive* item_interactive_da = Cast<UDA_Item_Interactive>(item_da);
	if (item_interactive_da)
		Montage_Interactive = item_interactive_da->Montage_Interactive;
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::Set_Bounds(const FBoxSphereBounds& bounds)
{
	Current_Bounds = bounds;

	Attack_Sphere_Info.Local_Top = Current_Bounds.Origin + FVector(0.0f, 0.0f, Current_Bounds.BoxExtent.Z);
	Attack_Sphere_Info.Local_Bottom = Current_Bounds.Origin - FVector(0.0f, 0.0f, Current_Bounds.BoxExtent.Z);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::Notify_Attack(const bool is_start)
{
	Can_Damaged = is_start;

	if (is_start)
	{
		Hit_Actors.Empty();		

		Attack_Collision_Tick();
	}
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::Attack_Collision_Tick()
{
	Skip_Attack_Frame = !Skip_Attack_Frame;

	if (Skip_Attack_Frame)
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UItem_Comp_Master::Attack_Collision_Tick);
	else if (Can_Damaged && Current_Hand_Comp)
	{
		bool is_hit = false;
		AActor* hit_actor = nullptr;
		float damage_value = 0.0f;

		const FVector& local_extend = Current_Bounds.BoxExtent;

		const FTransform mesh_transform = Current_Hand_Comp->GetComponentTransform();

		const FVector location = (mesh_transform.TransformPosition(Attack_Sphere_Info.Local_Top) + mesh_transform.TransformPosition(Attack_Sphere_Info.Local_Bottom)) * 0.5f;

		const FVector scale = Current_Hand_Comp->GetComponentScale();
		const float capsule_radius = FMath::Max(local_extend.X * scale.X, local_extend.Y * scale.Y);

		const float capsule_hald_height = local_extend.Z * scale.Z;

		TArray<FHitResult> out_hits = {};

		is_hit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(this, location, location, capsule_radius,
			capsule_hald_height, Attack_Sphere_Info.Object_Types, true, Hit_Actors,
			Show_Debug_Attack ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, out_hits, true, FLinearColor::Red, FLinearColor::Green, 0.33f);

		if (is_hit)
		{
			for (const FHitResult& result : out_hits)
			{
				hit_actor = result.GetActor();
				if (hit_actor && !Hit_Actors.Contains(hit_actor))
				{
					Hit_Actors.Add(hit_actor);

					UDA_Item_Interactive* da_item = Cast<UDA_Item_Interactive>(Item_DA);

					ensureMsgf(da_item, TEXT("nullptr!"));

					damage_value = da_item->Item_Damage;

					S_Apply_Damage(damage_value, hit_actor);
				}
			}
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UItem_Comp_Master::Attack_Collision_Tick);
	}
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::S_Apply_Damage_Implementation(const float damage, AActor* damage_causer)
{
	On_Apply_Damage.ExecuteIfBound(damage, damage_causer);
}
//------------------------------------------------------------------------------------------------------------
bool UItem_Comp_Master::Remove_And_Check_Same_Item(UItem_Master* item)
{
	if (Current_Hand_Comp)
	{
		Current_Hand_Comp->Deactivate();
		Current_Hand_Comp->DestroyComponent();
		Current_Hand_Comp = nullptr;
	}

	return Current_Item == item;
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Master::On_Attack_Montage_Ended(UAnimMontage* montage, bool is_interrupted)
{
	UDA_Item_Interactive* item_da = Cast<UDA_Item_Interactive>(Item_DA);
	if (!item_da || item_da && item_da->Montage_Interactive == montage)
	{
		ensureMsgf(item_da, TEXT("item_da = nullptr!"));

		Is_Attacking = false;

		On_Attacking.ExecuteIfBound(Is_Attacking);
		return;
	}
}

//------------------------------------------------------------------------------------------------------------
