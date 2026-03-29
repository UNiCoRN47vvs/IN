#include "Widgets/W_Recalculate_DA.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "UObject/Package.h"
#include "Data_Assets/DA_Loot_Container.h"
//------------------------------------------------------------------------------------------------------------
void UW_Recalculate_DA::NativeConstruct()
{
	Super::NativeConstruct();
}
//------------------------------------------------------------------------------------------------------------
void UW_Recalculate_DA::On_Clicked_Button_Recalculate_DA()
{
#if WITH_EDITOR

	FAssetRegistryModule& asset_registry_module = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	int32 recalculated_count = 0;
	UObject* asset = nullptr;
	UDA_Loot_Container* da_loot_container = nullptr;
	UPackage* package = nullptr;
	TArray<FAssetData> assets = {};

	asset_registry_module.Get().GetAssetsByClass(UDA_Loot_Container::StaticClass()->GetClassPathName(), assets);

	for (const FAssetData& asset_data : assets)
	{
		asset = asset_data.GetAsset();
		if (!asset)
			continue;

		da_loot_container = Cast<UDA_Loot_Container>(asset);
		if (!da_loot_container)
			continue;

		da_loot_container->Modify();
		da_loot_container->PostEditChange();

		package = da_loot_container->GetOutermost();
		if (package) // Помечаем пакет как изменённый
			package->SetDirtyFlag(true);
	}
#endif
}
//------------------------------------------------------------------------------------------------------------