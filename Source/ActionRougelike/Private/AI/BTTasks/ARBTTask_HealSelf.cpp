#include "AI/BTTasks/ARBTTask_HealSelf.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"

#include "Attributes/ARAttributeComponent.h"

EBTNodeResult::Type UARBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (APawn* MyPawn = AIController->GetPawn())
		{
			if (UARAttributeComponent* MyAttributeComponent = Cast<UARAttributeComponent>(MyPawn->GetComponentByClass(UARAttributeComponent::StaticClass())))
			{
				MyAttributeComponent->ApplyHealthChange(MyAttributeComponent->GetHealthMax());
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
