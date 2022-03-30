#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class ACTIONROUGELIKE_API UARWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ProjectPositionToPlayerScreen();

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TWeakObjectPtr<AActor> AttachedActor;

public:
	void SetupWidget(AActor* InAttachedActor);
};
