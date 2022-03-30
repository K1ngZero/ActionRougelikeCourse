#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "ARActionComponent.generated.h"

class UARAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API UARActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARActionComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<UARAction>> DefaultActions;

	UPROPERTY()
	TArray<UARAction*> Actions;

public:	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* InInstigator, TSubclassOf<UARAction> ActionClass);

	void RemoveAction(UARAction* InAction);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StartActionByName(AActor* InInstigator, const FName& ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StopActionByName(AActor* InInstigator, const FName& ActionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
};
