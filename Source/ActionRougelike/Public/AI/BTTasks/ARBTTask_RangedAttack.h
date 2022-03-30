#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARBTTask_RangedAttack.generated.h"

UCLASS()
class ACTIONROUGELIKE_API UARBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread = 2.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
};
