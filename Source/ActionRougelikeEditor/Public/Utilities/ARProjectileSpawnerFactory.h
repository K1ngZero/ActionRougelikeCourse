#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Factories/BlueprintFactory.h"
#include "ARProjectileSpawnerFactory.generated.h"

UCLASS()
class ACTIONROUGELIKEEDITOR_API UARProjectileSpawnerFactory : public UBlueprintFactory
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = ProjectileSpawner)
	TSubclassOf<UARProjectileSpawnerFactory> ProjectileSpawnerClass;

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

public:
	UARProjectileSpawnerFactory(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool IsMacroFactory() const { return false; }
};
