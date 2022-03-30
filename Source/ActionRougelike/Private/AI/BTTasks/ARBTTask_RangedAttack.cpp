#include "AI/BTTasks/ARBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

#include "Attributes/ARAttributeComponent.h"

EBTNodeResult::Type UARBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		if (ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn()))
		{
			if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor")))
			{
				if(!UARAttributeComponent::IsActorAlive(TargetActor))
				{
					return EBTNodeResult::Failed;
				}

				const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
				const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
				FRotator MuzzleRotation = Direction.Rotation();
				MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
				MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Instigator = MyPawn;
				SpawnParams.Owner = MyController;

				AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Failed;
}
