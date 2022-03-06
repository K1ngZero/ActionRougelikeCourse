#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

class UCameraComponent;
class UAnimMontage;
class USpringArmComponent;

class UARAttributeComponent;
class AARProjectile;
class UARInteractionComponent;

UCLASS()
class ACTIONROUGELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UARInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UARAttributeComponent* AttributeComponent = nullptr;

public:
	AARCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

protected:
	void MoveForward(float InAxis);

	void MoveRight(float InAxis);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void SecondaryAttack();

	void SecondaryAttack_TimeElapsed();

	void Dash();

	void Dash_TimeElapsed();

	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AARProjectile> InProjectileClass);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth);
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AARProjectile> PrimaryAttackProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AARProjectile> SecondaryAttackProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AARProjectile> DashProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnimation;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_SecondaryAttack;

	FTimerHandle TimerHandle_DashAttack;
};
