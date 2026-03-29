#include "Actor_Components/Items/Item_Comp_Melee.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Data_Assets/DA_Item_Tool.h"
#include "Data_Assets/DA_Item_Fist.h"
//------------------------------------------------------------------------------------------------------------
UItem_Comp_Melee::UItem_Comp_Melee()
{
	Attack_Mode = EAttack_Mode::Melee;
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Melee::Init_Item_Comp(UDA_Item_Master* item_da, USkeletalMeshComponent* owner_mesh, UItem_Master* item)
{
	Super::Init_Item_Comp(item_da, owner_mesh, item);

	if (UDA_Item_Tool* item_tool_da = Cast<UDA_Item_Tool>(item_da))
		Create_Comp_Melee(item_tool_da, owner_mesh);
	else if(UDA_Item_Fist* item_fist_da = Cast<UDA_Item_Fist>(item_da))
		Create_Comp_Fist(item_fist_da, owner_mesh);
	else
		CHECK_ALARM(false, TEXT("item_tool_da = nullptr!"));
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Melee::Create_Comp_Fist(UDA_Item_Fist* item_fist_da, USkeletalMeshComponent* owner_mesh)
{
	CHECK_PTR(owner_mesh);

	USphereComponent* sphere_component = NewObject<USphereComponent>(this, USphereComponent::StaticClass());

	CHECK_ALARM(sphere_component, TEXT("component nullptr!"));

	sphere_component->RegisterComponent();
	sphere_component->SetIsReplicated(true);
	sphere_component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	owner_mesh->GetOwner()->AddInstanceComponent(sphere_component);

	sphere_component->AttachToComponent(owner_mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, item_fist_da->Socket_Name);
	sphere_component->SetSphereRadius(item_fist_da->Fist_Radius_Size);

	Current_Hand_Comp = sphere_component;

	Set_Bounds(sphere_component->GetLocalBounds());
}
//------------------------------------------------------------------------------------------------------------
void UItem_Comp_Melee::Create_Comp_Melee(UDA_Item_Tool* item_tool_da, USkeletalMeshComponent* owner_mesh)
{
	if (!item_tool_da->Tool_Static_Mesh.Get())
	{
		TWeakObjectPtr<UItem_Comp_Melee> weak_this = this;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(item_tool_da->Tool_Static_Mesh.ToSoftObjectPath(), [weak_this, item_tool_da, owner_mesh]()
			{
				if(weak_this.IsValid())
					weak_this->Create_Comp_Melee(item_tool_da, owner_mesh);
			});

		return;
	}
	CHECK_PTR(owner_mesh);

	UStaticMeshComponent* static_mesh_component = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());

	CHECK_ALARM(static_mesh_component, TEXT("component nullptr!"));

	static_mesh_component->RegisterComponent();
	static_mesh_component->SetIsReplicated(true);
	static_mesh_component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	owner_mesh->GetOwner()->AddInstanceComponent(static_mesh_component);

	static_mesh_component->SetStaticMesh(item_tool_da->Tool_Static_Mesh.Get());
	static_mesh_component->AttachToComponent(owner_mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, item_tool_da->Socket_Name);

	Current_Hand_Comp = static_mesh_component;

	Set_Bounds(static_mesh_component->GetStaticMesh()->GetBounds());
}
//------------------------------------------------------------------------------------------------------------
