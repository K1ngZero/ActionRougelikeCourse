#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "ARActionComponent.generated.h"

class UARAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAROnActionStateChangedSignature, UARActionComponent*, OwningActionComponent, UARAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API UARActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARActionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* InInstigator, TSubclassOf<UARAction> ActionClass);

	void RemoveAction(UARAction* InAction);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* InInstigator, const FName& ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* InInstigator, const FName& ActionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
	FAROnActionStateChangedSignature OnActionStartedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAROnActionStateChangedSignature OnActionStoppedDelegate;

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, const FName& ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, const FName& ActionName);

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<UARAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UARAction*> Actions;

public:
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
