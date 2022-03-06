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

	AttributeComponent->OnHealthChangedDelegate.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ThisClass::MoveRight);

	PlayerInputComponent->BindAxis(FName("TurnX"), this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(FName("TurnY"), this, &ThisClass::AddControllerPitchInput);

	PlayerInputComponent->BindAction(FName("PrimaryAttack"), IE_Pressed, this, &ThisClass::PrimaryAttack);
	PlayerInputComponent->BindAction(FName("SecondaryAttack"), IE_Pressed, this, &ThisClass::SecondaryAttack);
	PlayerInputComponent->BindAction(FName("DashAttack"), IE_Pressed, this, &ThisClass::Dash);

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
	SpawnProjectile(PrimaryAttackProjectileClass);
}

void AARCharacter::SecondaryAttack()
{
	if (!TimerHandle_SecondaryAttack.IsValid())
	{
		PlayAnimMontage(AttackAnimation);
		GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ThisClass::SecondaryAttack_TimeElapsed, 0.2f);
	}
}

void AARCharacter::SecondaryAttack_TimeElapsed()
{
	TimerHandle_SecondaryAttack.Invalidate();
	SpawnProjectile(SecondaryAttackProjectileClass);
}

void AARCharacter::Dash()
{
	if (!TimerHandle_DashAttack.IsValid())
	{
		PlayAnimMontage(AttackAnimation);
		GetWorldTimerManager().SetTimer(TimerHandle_DashAttack, this, &ThisClass::Dash_TimeElapsed, 0.2f);
	}
}

void AARCharacter::Dash_TimeElapsed()
{
	TimerHandle_DashAttack.Invalidate();
	SpawnProjectile(DashProjectileClass);
}

void AARCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void AARCharacter::SpawnProjectile(TSubclassOf<AARProjectile> InProjectileClass)
{
	if (!ensureAlways(InProjectileClass))
	{
		return;
	}

	const FCollisionShape TraceShape(FCollisionShape::MakeSphere(20.0f));
	const FCollisionQueryParams TraceCollisionParams(FName("CharacterSpawnProjectile"), false, this);
	const FCollisionObjectQueryParams TraceObjectParams(ECC_TO_BITFIELD(ECC_WorldDynamic | ECC_WorldStatic | ECC_Pawn));

	const FVector HandLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	const FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComponent->GetForwardVector() * 100000.0f;

	FHitResult OutResult;
	if (GetWorld()->SweepSingleByObjectType(OutResult, TraceStart, TraceEnd, FQuat::Identity, TraceObjectParams, TraceShape, TraceCollisionParams))
	{
		TraceEnd = OutResult.Location;
	}

	const FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	const FTransform ProjectileSpawnTransform = FTransform(ProjectileRotation, HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = GetController();

	GetWorld()->SpawnActor<AARProjectile>(InProjectileClass, ProjectileSpawnTransform, SpawnParameters);
}

void AARCharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth)
{
	if (InNewHealth <= 0.0f && InOldHealth > 0.0f)
	{
		DisableInput(GetController<APlayerController>());
	}
}