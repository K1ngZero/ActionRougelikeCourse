#include "Character/ARCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComponent"));
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
	PlayerInputComponent->BindAction(FName("PrimaryInteract"), IE_Pressed, this, &ThisClass::PrimaryInteract);
}

void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = CameraComponent->GetComponentLocation();
	OutRotation = CameraComponent->GetComponentRotation();
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
	if (!TimerHandle_PrimaryAttack.IsValid())
	{
		PlayAnimMontage(AttackAnimation);
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ThisClass::PrimaryAttack_TimeElapsed, 0.2f);
	}
}

void AARCharacter::PrimaryAttack_TimeElapsed()
{
	TimerHandle_PrimaryAttack.Invalidate();

	FVector HandLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComponent->GetForwardVector() * 100000.0f;

	FHitResult OutResult;
	FVector HitLocation = [&]() -> FVector
	{
		if (GetWorld()->LineTraceSingleByChannel(OutResult, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldStatic))
		{
			return OutResult.Location;
		}
		return TraceEnd;
	}();

	FRotator ProjectileRotation = FRotationMatrix::MakeFromX(HitLocation - HandLocation).Rotator();

	FTransform ProjectileSpawnTransform = FTransform(ProjectileRotation, HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = GetController();

	GetWorld()->SpawnActor<AARProjectile>(ProjectileClass, ProjectileSpawnTransform, SpawnParameters);
}

void AARCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}
