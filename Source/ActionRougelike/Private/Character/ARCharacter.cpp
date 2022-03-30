#include "Character/ARCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ActionSystem/ARActionComponent.h"
#include "Attributes/ARAttributeComponent.h"
#include "Interactive/ARInteractionComponent.h"
#include "Projectile/ARProjectile.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UARInteractionComponent>(TEXT("InteractionComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ThisClass::MoveRight);

	PlayerInputComponent->BindAxis(FName("TurnX"), this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(FName("TurnY"), this, &ThisClass::AddControllerPitchInput);

	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &ThisClass::SprintStart);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Released, this, &ThisClass::SprintStop);

	PlayerInputComponent->BindAction(FName("PrimaryAttack"), IE_Pressed, this, &ThisClass::PrimaryAttack);
	PlayerInputComponent->BindAction(FName("SecondaryAttack"), IE_Pressed, this, &ThisClass::SecondaryAttack);
	PlayerInputComponent->BindAction(FName("DashAttack"), IE_Pressed, this, &ThisClass::Dash);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("PrimaryInteract"), IE_Pressed, this, &ThisClass::PrimaryInteract);
}

void AARCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetPawnViewLocation();
	OutRotation = CameraComponent->GetComponentRotation();
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void AARCharacter::MoveForward(float InAxis)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), InAxis);
}

void AARCharacter::MoveRight(float InAxis)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, InAxis);
}

void AARCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AARCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void AARCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AARCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, "SecondaryAttack");
}

void AARCharacter::Dash()
{
	ActionComponent->StartActionByName(this, "Dash");
}

void AARCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void AARCharacter::OnCharacterDied()
{
	DisableInput(GetController<APlayerController>());
	Super::OnCharacterDied();
}

void AARCharacter::HealSelf(float Amount /*= 100.0f*/)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}