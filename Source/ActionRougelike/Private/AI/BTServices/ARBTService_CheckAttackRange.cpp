#include "AI/BTServices/ARBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (AActor* MyTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("TargetActor"))))
		{
			if (AAIController* MyController = OwnerComp.GetAIOwner())
			{
				if (APawn* AIPawn = MyController->GetPawn())
				{
					const float DistanceToTarget = FVector::Distance(MyTarget->GetActorLocation(), AIPawn->GetActorLocation());
					const bool bIsTargetWithinRange = DistanceToTarget < 1400.0f;
					const bool bIsTargetInRangeAndLOS = bIsTargetWithinRange && MyController->LineOfSightTo(MyTarget);

					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bIsTargetInRangeAndLOS);
				}
			}
		}
	}
}
