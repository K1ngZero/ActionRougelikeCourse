#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARAnimInstance.generated.h"

class UARActionComponent;

UCLASS()
class ACTIONROUGELIKE_API UARAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned = false;

	UPROPERTY(Transient)
	UARActionComponent* ActionComponent;
};
