#include "Character/ARCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(FName("TurnX"), this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(FName("TurnY"), this, &ThisClass::AddControllerPitchInput);
}

void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARCharacter::MoveForward(float InAxis)
{
	AddMovementInput(GetActorForwardVector(), InAxis);
}

void AARCharacter::MoveRight(float InAxis)
{
	AddMovementInput(GetActorRightVector(), InAxis);
}

