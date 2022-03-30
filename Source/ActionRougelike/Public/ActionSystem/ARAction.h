#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"

#include "ARAction.generated.h"

class UARActionComponent;

UCLASS(Blueprintable)
class ACTIONROUGELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

public:
	void OnActionAdded(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InInstigator) const;

	UFUNCTION(Category = "Action")
	bool IsRunning() const { return bIsRunning; }

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintPure, Category = "Action")
	UARActionComponent* GetOwningComponent() const;

	/* Tags applied to OwningActor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;
	
	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart = false;

	bool bIsRunning = false;
};
