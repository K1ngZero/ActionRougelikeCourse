#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacterBase.generated.h"

class UARAttributeComponent;

UCLASS()
class ACTIONROUGELIKE_API AARCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UARAttributeComponent* AttributeComponent = nullptr;

public:
	AARCharacterBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth);

	virtual void OnCharacterDied();
};
