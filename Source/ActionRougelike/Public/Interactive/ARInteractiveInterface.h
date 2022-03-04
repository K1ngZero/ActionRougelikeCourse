#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARInteractiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class UARInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

class ACTIONROUGELIKE_API IARInteractiveInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(APawn* InstigatorPawn);
};
