#include "Utilities/ARProjectileSpawnerFactory.h"
#include "ClassViewerModule.h"
#include "Projectile/ARProjectileSpawner.h"
#include "ClassViewerFilter.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"

#define LOCTEXT_NAMESPACE "ProjectileSpawner"

UARProjectileSpawnerFactory::UARProjectileSpawnerFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UARProjectileSpawner::StaticClass();
}

UObject* UARProjectileSpawnerFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UARProjectileSpawner::StaticClass()));
	return NewObject<UARProjectileSpawner>(InParent, Class, Name, Flags);
}

#undef LOC_NAMESPACE