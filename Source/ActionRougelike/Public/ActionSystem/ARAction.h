#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"

#include "ARAction.generated.h"

class UARActionComponent;

USTRUCT()
struct FARActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning = false;

	UPROPERTY(Transient)
	AActor* Instigator = nullptr;
};

UCLASS(Blueprintable)
class ACTIONROUGELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UARActionComponent* InActionComponent);

	void OnActionAdded(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InInstigator) const;

	UFUNCTION(Category = "Action")
	bool IsRunning() const;

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override { return true; }

protected:
	UFUNCTION(BlueprintPure, Category = "Action")
	UARActionComponent* GetOwningComponent() const { return MyActionComponent; }

	UPROPERTY(Transient, Replicated)
	UARActionComponent* MyActionComponent;

	/* Tags applied to OwningActor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;
	
	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart = false;

	UPROPERTY(ReplicatedUsing = OnRep_RepData)
	FARActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> MyInstigator;
};
