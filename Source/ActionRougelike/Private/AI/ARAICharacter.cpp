#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
}

void AARAICharacter::OnPawnSeen(APawn* InPawn)
{
	if (AAIController* AIController = GetController<AAIController>())
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

		BlackboardComponent->SetValueAsObject("TargetActor", InPawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Red, 4.0f, true);
	}
}

void AARAICharacter::OnCharacterDied()
{
	if (AAIController* AIController = GetController<AAIController>())
	{
		AIController->GetBrainComponent()->StopLogic("Character Died");
	}

	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	SetLifeSpan(FMath::RandRange(8.0f, 12.0f));
}