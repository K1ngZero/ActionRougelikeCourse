#include "UserInterface/ARWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UARWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!AttachedActor)
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
	const bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), WidgetPosition, ScreenPosition);

	if (bIsOnScreen)
	{
		const float DPIScaling = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= DPIScaling;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UARWorldUserWidget::SetupWidget(AActor* InAttachedActor)
{
	AttachedActor = InAttachedActor;
}