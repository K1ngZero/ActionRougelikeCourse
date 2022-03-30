#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ARAction.h"
#include "ARAction_ProjectileAttack.generated.h"

class UAnimMontage;

class AARProjectile;

UCLASS()
class ACTIONROUGELIKE_API UARAction_ProjectileAttack : public UARAction
{
	GENERATED_BODY()
	
public:
	UARAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AARProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName ProjectileSocketName = "Muzzle_01";

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float ProjectileAnimDelay = 0.2f;
};
