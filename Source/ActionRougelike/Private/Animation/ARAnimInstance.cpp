#include "Animation/ARAnimInstance.h"

#include "GameplayTagContainer.h"

#include "ActionSystem/ARActionComponent.h"

void UARAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		ActionComponent = Cast<UARActionComponent>(OwningActor->GetComponentByClass(UARActionComponent::StaticClass()));
	}
}

void UARAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	static FGameplayTag SprintingTag = FGameplayTag::RequestGameplayTag("Action.Sprinting");

	if (ActionComponent)
	{
		bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
		bIsSprinting = ActionComponent->ActiveGameplayTags.HasTag(SprintingTag);
	}
}
