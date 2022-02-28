#include "Character/ARCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Projectile/ARProjectile.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
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

	PlayerInputComponent->BindAction(FName("PrimaryAttack"), IE_Pressed, this, &ThisClass::PrimaryAttack);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
}

void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AARCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	FTransform ProjectileSpawnTransform = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = GetController();
	
	GetWorld()->SpawnActor<AARProjectile>(ProjectileClass, ProjectileSpawnTransform, SpawnParameters);
}

