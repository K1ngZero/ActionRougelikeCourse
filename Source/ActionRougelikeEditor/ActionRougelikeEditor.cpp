#include "ActionRougelikeEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "AssetTypeActions_Base.h"
#include "Projectile/ARProjectileSpawner.h"
#include "AssetTypeCategories.h"
#include "../Plugins/Compositing/Composure/Source/ComposureLayersEditor/Private/ComposureEditorStyle.h"
#include "Interfaces/IPluginManager.h"

IMPLEMENT_GAME_MODULE(FActionRougelikeEditor, ActionRougelikeEditor);

DEFINE_LOG_CATEGORY(ActionRougelikeEditorLog)

#define LOCTEXT_NAMESPACE "ActionRougelikeEditor"

static EAssetTypeCategories::Type Objectives;

class FUI_ProjSpawner : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override { return LOCTEXT("ProjectileSpawner", "Projectile Spawner"); }
	virtual uint32 GetCategories() override { return Objectives; }
	virtual FColor GetTypeColor() const override { return FColor(255, 0, 55); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override { return LOCTEXT("ProjectileSpawnerDesc", "Projectile Spawner lool"); }
	virtual UClass* GetSupportedClass() const override { return UARProjectileSpawner::StaticClass(); }
};

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

void FActionRougelikeEditor::StartupModule()
{
	UE_LOG(ActionRougelikeEditorLog, Warning, TEXT("ActionRougelikeEditor: Log Started"));

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	Objectives = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Objectives")), LOCTEXT("Objectives", "Objectives"));
	{
		TSharedRef<IAssetTypeActions> ACT_UMyItemsDatabase = MakeShareable(new FUI_ProjSpawner);
		AssetTools.RegisterAssetTypeActions(ACT_UMyItemsDatabase);
	}

	StyleSet = MakeUnique<FSlateStyleSet>(FName(TEXT("ProjSpawnerStyle")));
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));


	StyleSet->Set("ClassIcon.ARProjectileSpawner", new IMAGE_BRUSH("Icons/AssetIcons/PaperSprite_16x", FVector2D(16.0f, 16.0f)));
	StyleSet->Set("ClassThumbnail.ARProjectileSpawner", new IMAGE_BRUSH("Icons/AssetIcons/PaperSprite_64x", FVector2D(64.0f, 64.0f)));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FActionRougelikeEditor::ShutdownModule()
{
	UE_LOG(ActionRougelikeEditorLog, Warning, TEXT("ActionRougelikeEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE