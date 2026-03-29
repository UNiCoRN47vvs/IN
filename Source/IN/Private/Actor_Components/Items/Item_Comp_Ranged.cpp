#include "Actor_Components/Items/Item_Comp_Ranged.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Data_Assets/DA_Item_Equippable.h"
//------------------------------------------------------------------------------------------------------------
UItem_Comp_Ranged::UItem_Comp_Ranged()
{
	Attack_Mode = EAttack_Mode::Ranged;
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Ranged::Init_Item_Comp(UDA_Item_Master* item_da, USkeletalMeshComponent* owner_mesh, UItem_Master* item)
{
	Super::Init_Item_Comp(item_da, owner_mesh, item);

	CHECK_ALARM(item_da, TEXT("item_da = nullptr"));

	UDA_Item_Equippable* item_equippable_da = Cast<UDA_Item_Equippable>(item_da);
	CHECK_ALARM(item_equippable_da, TEXT("item_da = nullptr"));

	Create_Comp_Ranged(item_equippable_da, owner_mesh);
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Ranged::Create_Comp_Ranged(UDA_Item_Equippable* item_equippable_da, USkeletalMeshComponent* owner_mesh)
{
	if (!item_equippable_da->Equippable_Skeletal_Mesh.Get())
	{
		TWeakObjectPtr<UItem_Comp_Ranged> weak_this = this;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(item_equippable_da->Equippable_Skeletal_Mesh.ToSoftObjectPath(), [weak_this, item_equippable_da, owner_mesh]()
			{
				if (weak_this.IsValid())
					weak_this->Create_Comp_Ranged(item_equippable_da, owner_mesh);
			});

		return;
	}

	CHECK_PTR(owner_mesh);

	USkeletalMeshComponent* skeletal_mesh_component = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass());

	CHECK_ALARM(skeletal_mesh_component, TEXT("component nullptr!"));

	skeletal_mesh_component->RegisterComponent();
	skeletal_mesh_component->SetIsReplicated(true);
	skeletal_mesh_component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	owner_mesh->GetOwner()->AddInstanceComponent(skeletal_mesh_component);

	skeletal_mesh_component->SetSkeletalMesh(item_equippable_da->Equippable_Skeletal_Mesh.Get());
	skeletal_mesh_component->AttachToComponent(owner_mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, item_equippable_da->Socket_Name);

	Current_Hand_Comp = skeletal_mesh_component;

	Set_Bounds(skeletal_mesh_component->GetSkeletalMeshAsset()->GetBounds());
}
//------------------------------------------------------------------------------------------------------------
