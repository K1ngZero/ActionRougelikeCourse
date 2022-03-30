#include "UserInterface/ARWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UARWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!AttachedActor.IsValid())
	{
		RemoveFromParent();
		return;
	}

	ProjectPositionToPlayerScreen();
	return;
}

void UARWorldUserWidget::ProjectPositionToPlayerScreen()
{
	FVector2D ScreenPosition;

	FVector ActorOrigin, ActorExtent;
	AttachedActor->GetActorBounds(true, ActorOrigin, ActorExtent);

	const FVector WidgetPosition = ActorOrigin + FVector(0, 0, ActorExtent.Z);

	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), WidgetPosition, ScreenPosition))
	{
		const float DPIScaling = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= DPIScaling;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}

void UARWorldUserWidget::SetupWidget(AActor* InAttachedActor)
{
	AttachedActor = InAttachedActor;
}