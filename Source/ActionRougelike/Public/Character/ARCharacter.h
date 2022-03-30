#pragma once

#include "CoreMinimal.h"

#include "Character/ARCharacterBase.h"

#include "ARCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class UARInteractionComponent;

UCLASS()
class ACTIONROUGELIKE_API AARCharacter : public AARCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UARInteractionComponent* InteractionComponent = nullptr;

public:
	AARCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	virtual FVector GetPawnViewLocation() const override;

protected:
	void MoveForward(float InAxis);

	void MoveRight(float InAxis);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void SecondaryAttack();

	void Dash();

	void PrimaryInteract();

	virtual void OnCharacterDied() override;

public:
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);
};
