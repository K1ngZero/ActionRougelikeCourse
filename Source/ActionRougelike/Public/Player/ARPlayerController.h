#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAROnPawnChangedSignature, APawn*, NewPawn, APawn*, OldPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAROnPlayerStateChangedSignature, APlayerState*, NewPlayerState);

UCLASS()
class ACTIONROUGELIKE_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetPawn(APawn* InPawn) override;

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "BeginPlayingState")
	void BP_BeginPlayingState();

	virtual void OnRep_PlayerState() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	
	UPROPERTY(BlueprintAssignable)
	FAROnPawnChangedSignature OnPawnChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAROnPlayerStateChangedSignature OnPlayerStateChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(Transient)
	UUserWidget* PauseMenuWidget;
};
