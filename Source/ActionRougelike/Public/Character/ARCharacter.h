#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class AARProjectile;

UCLASS()
class ACTIONROUGELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent = nullptr;

public:
	AARCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

protected:
	void MoveForward(float InAxis);

	void MoveRight(float InAxis);

	void PrimaryAttack();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AARProjectile> ProjectileClass;
};
