#include "Interactive/ARInteractionComponent.h"

#include "DrawDebugHelpers.h"

#include "Interactive/ARInteractiveInterface.h"
#include "UserInterface/ARWorldUserWidget.h"

namespace ARInteractionCVars
{
	static TAutoConsoleVariable<bool> CVarInteractionShowDebug(
		TEXT("ar.Interaction.ShowDebug"),
		false,
		TEXT("Show debug for interactions."),
		ECVF_Cheat);
}

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
	UpdateInteractionWidget();
}

void UARInteractionComponent::FindBestInteractable()
{
	TArray<FHitResult> OutHitResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape Shape;
	Shape.SetSphere(InteractionTraceRadius);

	FVector LinetraceStart;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(LinetraceStart, EyeRotation);

	const FVector LinetraceEnd = LinetraceStart + EyeRotation.Vector() * InteractionTraceRange;
	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(OutHitResults, LinetraceStart, LinetraceEnd, FQuat::Identity, ObjectQueryParams, Shape);
	const FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	AActor* FoundInteractable = nullptr;

	for (const FHitResult& Hit : OutHitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UARInteractiveInterface>())
			{
				FoundInteractable = HitActor;
				break;
			}
		}

		if (ARInteractionCVars::CVarInteractionShowDebug.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 5.0f, 12, LineColor, false, 2.0f);
		}
	}

	if (FocusedInteractable != FoundInteractable)
	{
		FocusedInteractable = FoundInteractable;
	}

	if (ARInteractionCVars::CVarInteractionShowDebug.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), LinetraceStart, LinetraceEnd, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void UARInteractionComponent::UpdateInteractionWidget()
{
	if (FocusedInteractable.IsValid())
	{
		if (DefaultWidgetClass && !DefaultWidgetInstance)
		{
			DefaultWidgetInstance = CreateWidget<UARWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->SetupWidget(FocusedInteractable.Get());

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void UARInteractionComponent::PrimaryInteract()
{
	if (FocusedInteractable.IsValid())
	{
		APawn* MyPawn = Cast<APawn>(GetOwner());
		IARInteractiveInterface::Execute_Interact(FocusedInteractable.Get(), MyPawn);
	}
}