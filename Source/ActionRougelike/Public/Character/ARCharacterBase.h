#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacterBase.generated.h"

class UARActionComponent;
class UARAttributeComponent;

UCLASS()
class ACTIONROUGELIKE_API AARCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UARActionComponent* ActionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UARAttributeComponent* AttributeComponent = nullptr;

public:
	AARCharacterBase();

	int32 GetKillBounty() const { return KillBounty; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth);

	virtual void OnCharacterDied();

	UPROPERTY(EditDefaultsOnly)
	int32 KillBounty = 5;

	UPROPERTY(EditDefaultsOnly)
	float DamageTakenFractionAsRage = 0.25f;
};
