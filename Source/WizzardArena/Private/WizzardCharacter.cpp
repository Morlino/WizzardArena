// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "WizzardHUD.h"
#include "WizzardPlayerController.h"

// Sets default values
AWizzardCharacter::AWizzardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 800.0f;
	SpringArmComp->bDoCollisionTest = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Combat
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh());
	ProjectileSpawnPoint->SetRelativeLocation(FVector(-30.0f, 40.0f, 120.0f));

	CurrentHealth = MaxHealth;

	// HUD
	
}

// Called when the game starts or when spawned
void AWizzardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWizzardCharacter::InitHUD()
{
	// Set starting health
	WizzardHUD->SetHealth(CurrentHealth, MaxHealth);
}

void AWizzardCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	// Log the input values
	UE_LOG(LogTemp, Log, TEXT("Move Input: X=%.2f, Y=%.2f"), MovementVector.X, MovementVector.Y);

	AddMovementInput(GetActorRightVector(), MovementVector.X);
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
}

void AWizzardCharacter::RotateToCursor()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector PlaneIntersection = FMath::LinePlaneIntersection(
			WorldLocation,
			WorldLocation + WorldDirection * 1000.0f,
			FVector(0.0f, 0.0f, GetActorLocation().Z),
			FVector::UpVector
		);

		// Save cursor location
		CursorWorldLocation = PlaneIntersection;

		FVector LookDir = PlaneIntersection - GetActorLocation();
		LookDir.Z = 0.0f;

		if (!LookDir.IsNearlyZero())
		{
			FRotator TargetRotation = LookDir.Rotation();
			TargetRotation.Pitch = 0.f;
			TargetRotation.Roll = 0.f;
			FRotator MeshOffset(MeshRotationOffset);
			GetMesh()->SetWorldRotation(TargetRotation + MeshOffset);
		}
	}
}

// Called every frame
void AWizzardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToCursor();

	FireCurrentCooldown -= DeltaTime;
	if (bIsFiring)
	{
		if (FireCurrentCooldown <= 0.0f)
		{
			ShootProjectile();
			FireCurrentCooldown = FireCooldown;
		}
	}
}

void AWizzardCharacter::SetHUDReference(UWizzardHUD* HUD)
{
	WizzardHUD = HUD;

	// Optional: initialize HUD values immediately
	if (WizzardHUD)
	{
		WizzardHUD->SetHealth(CurrentHealth, MaxHealth);
	}
}

void AWizzardCharacter::ShootProjectile()
{
	if (!ProjectileClass) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FVector ShootDir = (CursorWorldLocation - SpawnLocation).GetSafeNormal2D();
	FRotator SpawnRotation = ShootDir.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	UE_LOG(LogTemp, Log, TEXT("Projectile spawned at %s"), *SpawnLocation.ToString());
}

void AWizzardCharacter::StartFiring()
{
	UE_LOG(LogTemp, Log, TEXT("StartFiring"));
	bIsFiring = true;
}

void AWizzardCharacter::StopFiring()
{
	UE_LOG(LogTemp, Log, TEXT("StopFiring"));
	bIsFiring = false;
}

// Called to bind functionality to input
void AWizzardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup Action Binding
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWizzardCharacter::Move);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AWizzardCharacter::StartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AWizzardCharacter::StopFiring);
	}
}

void AWizzardCharacter::Die()
{
	UE_LOG(LogTemp, Log, TEXT("You Died"));

	Destroy();
}

void AWizzardCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);

	if (WizzardHUD)
	{
		WizzardHUD->SetHealth(CurrentHealth, MaxHealth);
	}

	if (CurrentHealth <= 0.f)
	{
		Die();
	}
}
