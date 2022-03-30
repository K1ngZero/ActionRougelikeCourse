#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARGameplayFunctionLibrary.generated.h"

UCLASS()
class ACTIONROUGELIKE_API UARGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* TargetActor, AActor* DamageCauser, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* TargetActor, AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static int32 GetKillBounty(AActor* InVictimActor);
};
