#include "AI/BTTasks/ARBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UARBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		if (ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn()))
		{
			if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor")))
			{
				const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
				const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
				const FRotator MuzzleRotation = Direction.Rotation();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Failed;
}
