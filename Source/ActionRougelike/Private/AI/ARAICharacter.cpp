#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

#include "UserInterface/ARWorldUserWidget.h"

AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetGenerateOverlapEvents(true);
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
}

void AARAICharacter::OnPawnSeen(APawn* InPawn)
{
	SetTargetActor(InPawn);
	MulticastOnPawnSeen();
}

void AARAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (AAIController* AIController = GetController<AAIController>())
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AARAICharacter::MulticastOnPawnSeen_Implementation()
{
	if (!PlayerSpottedWidget && PlayerSpottedWidgetClass)
	{
		PlayerSpottedWidget = CreateWidget<UARWorldUserWidget>(UGameplayStatics::GetPlayerController(this, 0), PlayerSpottedWidgetClass);
		PlayerSpottedWidget->SetupWidget(this);
		PlayerSpottedWidget->AddToViewport();
	}
}

void AARAICharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth)
{
	if (InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	if (InNewHealth < InOldHealth)
	{
		if (!HealthBarWidget && HealthBarWidgetClass)
		{
			HealthBarWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (HealthBarWidget)
			{
				HealthBarWidget->SetupWidget(this);
				HealthBarWidget->AddToViewport();
			}
		}
	}

	Super::OnHealthChanged(InstigatorActor, OwningComponent, InNewHealth, InOldHealth);
}

void AARAICharacter::OnCharacterDied()
{
	if (AAIController* AIController = GetController<AAIController>())
	{
		AIController->GetBrainComponent()->StopLogic("Character Died");
	}

	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	Super::OnCharacterDied();
}