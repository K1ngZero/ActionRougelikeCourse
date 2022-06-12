#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAROnPawnChangedSignature, APawn*, NewPawn, APawn*, OldPawn);

UCLASS()
class ACTIONROUGELIKE_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetPawn(APawn* InPawn) override;

	UPROPERTY(BlueprintAssignable)
	FAROnPawnChangedSignature OnPawnChangedDelegate;
};
