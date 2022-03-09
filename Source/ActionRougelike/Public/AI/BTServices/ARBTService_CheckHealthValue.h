#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARBTService_CheckHealthValue.generated.h"

UCLASS()
class ACTIONROUGELIKE_API UARBTService_CheckHealthValue : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float HealthThreshold = 30.0f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector KeyToSet;
};
