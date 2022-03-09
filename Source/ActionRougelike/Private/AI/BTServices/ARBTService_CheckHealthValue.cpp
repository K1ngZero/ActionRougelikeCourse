#include "AI/BTServices/ARBTService_CheckHealthValue.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

#include "Attributes/ARAttributeComponent.h"

void UARBTService_CheckHealthValue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AAIController* MyController = OwnerComp.GetAIOwner())
	{
		if (APawn* MyPawn = MyController->GetPawn())
		{
			if (UARAttributeComponent* MyAttributeComponent = Cast<UARAttributeComponent>(MyPawn->GetComponentByClass(UARAttributeComponent::StaticClass())))
			{
				if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
				{
					const bool bIsBelowHealthThreshold = MyAttributeComponent->GetHealth() < HealthThreshold;
					BlackboardComponent->SetValueAsBool(KeyToSet.SelectedKeyName, bIsBelowHealthThreshold);
				}
			}
		}
	}
}
