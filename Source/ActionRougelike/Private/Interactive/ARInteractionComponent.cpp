#include "Interactive/ARInteractionComponent.h"

#include "DrawDebugHelpers.h"

#include "Interactive/ARInteractiveInterface.h"

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARInteractionComponent::PrimaryInteract()
{
	TArray<FHitResult> OutHitResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector LinetraceStart;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(LinetraceStart, EyeRotation);

	FVector LinetraceEnd = LinetraceStart + EyeRotation.Vector() * 500.0f;

	FCollisionShape Shape;
	Shape.SetSphere(30.0f);

	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(OutHitResults, LinetraceStart, LinetraceEnd, FQuat::Identity, ObjectQueryParams, Shape);
	const FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (const FHitResult& Hit : OutHitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UARInteractiveInterface>())
			{
				APawn* MyPawn = Cast<APawn>(GetOwner());

				IARInteractiveInterface::Execute_Interact(HitActor, MyPawn);

				break;
			}
		}

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 5.0f, 12, LineColor, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), LinetraceStart, LinetraceEnd, LineColor, false, 2.0f, 0, 2.0f);
}