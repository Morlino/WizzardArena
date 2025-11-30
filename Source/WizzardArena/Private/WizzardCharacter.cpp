// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "WizzardGameMode.h"
#include "WizzardHUD.h"
#include "WizzardPlayerController.h"
#include "WizzardProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWizzardCharacter::AWizzardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = TargetArmLength;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 2.0f;
	SpringArmComp->bInheritYaw = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->FieldOfView = 75.0f;

	// Projectile
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh());
	ProjectileSpawnPoint->SetRelativeLocation(FVector(-30.0f, 40.0f, 120.0f));

	// Dash Collision
	DashCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DashCollision"));
	DashCollisionSphere->SetupAttachment(RootComponent);
	DashCollisionSphere->SetSphereRadius(DashDamageRadius);
	DashCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DashCollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	DashCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWizzardCharacter::OnDashOverlap);

	// Barrier
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(GetRootComponent());
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldMesh->SetVisibility(false);

	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void AWizzardCharacter::BeginPlay()
{
	Super::BeginPlay();

	BaseProjectileCastTime = ProjectileCastTime;
	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	OnHealthChanged.AddDynamic(this, &AWizzardCharacter::UpdateHealthHUD);
}

void AWizzardCharacter::UpdateHealthHUD(float NewHealth, float NewMaxHealth)
{
	UE_LOG(LogTemp, Log, TEXT("Delegate fired! New Health=%f"), NewHealth);

	WizzardHUD->SetHealth(NewHealth, NewMaxHealth);
}

void AWizzardCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FRotator ControlRot = PC->GetControlRotation();
	// Flatten pitch to prevent movement going up/down
	ControlRot.Pitch = 0.f;

	// Convert to forward/right vectors
	const FVector Forward = UKismetMathLibrary::GetForwardVector(ControlRot);
	const FVector Right   = UKismetMathLibrary::GetRightVector(ControlRot);

	// Apply movement
	AddMovementInput(Forward, MovementVector.Y); // W/S
	AddMovementInput(Right, MovementVector.X);   // A/D
}


void AWizzardCharacter::UpdateCursorWorldLocation()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Intersect with plane at character Z
		CursorWorldLocation = FMath::LinePlaneIntersection(
			WorldLocation,
			WorldLocation + WorldDirection * 10000.0f,
			FVector(0.0f, 0.0f, GetActorLocation().Z),
			FVector::UpVector
		);
	}
}

void AWizzardCharacter::RototatePlayerToCursor()
{
	FHitResult Hit;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// Convert ECC to ETraceTypeQuery for this function
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	// This calls the real function you referenced
	if (PC->GetHitResultUnderCursorByChannel(TraceType, true, Hit))
	{
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Hit.Location);

		FRotator NewRotation(GetActorRotation().Pitch, LookRot.Yaw, GetActorRotation().Roll);
		SetActorRotation(NewRotation);
	}
}

// Called every frame
void AWizzardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasWon || HasLost)
		return;

	// Cooldowns decrement
	FireCurrentCooldown -= DeltaTime;

	if (bIsDashing)
	{
		HandleDash(DeltaTime);
	}

	// RotateToCursor();
	UpdateCursorWorldLocation();
	RototatePlayerToCursor();
}

void AWizzardCharacter::Die_Implementation()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		DisableInput(PC);

		// Start fade out
		PC->PlayerCameraManager->StartCameraFade(
			0.0f,     // From alpha
			1.0f,     // To alpha
			4.0f,    // Duration (seconds)
			FLinearColor::Black, // Fade color
			false,   // bShouldFadeAudio
			true     // bHoldWhenFinished
		);

		// After fade duration, restart the level
		FTimerHandle RestartTimerHandle;
		GetWorldTimerManager().SetTimer(RestartTimerHandle, [this, PC]()
		{
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}, 4.0f, false); // matches fade duration
	}
	GetCharacterMovement()->DisableMovement();

	if (bIsFiring)
	{
		StopFiring();
	}

	if (AWizzardGameMode* GM = Cast<AWizzardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnGameLost.Broadcast();
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);

	UGameplayStatics::PlaySound2D(this, DeathSound);
	
	HasLost = true;
}

void AWizzardCharacter::Heal(float HealingAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealingAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("%s healed to %f"), *GetName(), CurrentHealth);
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

	if (bIsDashing) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FVector ShootDir = (CursorWorldLocation - SpawnLocation).GetSafeNormal2D();
	FRotator SpawnRotation = ShootDir.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AWizzardProjectile* Projectile = GetWorld()->SpawnActor<AWizzardProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		// Set projectile damage from character
		Projectile->SetDamage(Damage);
		Projectile->SetPushStrength(PushStrength);
		Projectile->SetProjectileOwner(this);  
	}

	UE_LOG(LogTemp, Log, TEXT("Projectile spawned at %s"), *SpawnLocation.ToString());
}

void AWizzardCharacter::PlayCastMontageIfPossible()
{
	if (!CastAnimation) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// Play only if not already playing
	if (!AnimInstance->Montage_IsPlaying(CastAnimation))
	{
		float Speed = CastAnimation->GetPlayLength() / ProjectileCastTime;
		PlayAnimMontage(CastAnimation, Speed);
	}
}

void AWizzardCharacter::StartFiring()
{
	bIsFiring = true;

	PlayCastMontageIfPossible();

	TryFire();
}

void AWizzardCharacter::StopFiring()
{
	UE_LOG(LogTemp, Log, TEXT("StopFiring"));
	bIsFiring = false;
}

void AWizzardCharacter::TryFire()
{
	if (!bIsFiring || !bCanFire)
		return;

	bCanFire = false;

	// If animation finished naturally before cooldown -> restart it
	PlayCastMontageIfPossible();

	// Start cooldown
	GetWorldTimerManager().SetTimer(
		FireCooldownTimer,
		this,
		&AWizzardCharacter::ResetFireCooldown,
		ProjectileCastTime,
		false
	);
}

void AWizzardCharacter::ResetFireCooldown()
{
	bCanFire = true;

	// If the player is still holding fire, shoot again
	TryFire();
}

void AWizzardCharacter::StartDash()
{
	if (!bCanDash || bIsDashing) return;

	UE_LOG(LogTemp, Log, TEXT("StartDash"));

	bCanDash = false;

	if (!DashAnimation) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// Play only if not already playing
	if (!AnimInstance->Montage_IsPlaying(DashAnimation))
	{
		float Speed = DashAnimation->GetPlayLength() / DashDuration;
		PlayAnimMontage(DashAnimation, Speed);
	}

	// Start cooldown timer
	GetWorldTimerManager().SetTimer(
		DashCooldownTimer,
		this,
		&AWizzardCharacter::ResetDashCooldown,
		DashCooldown,
		false
	);

	FVector DashDir = FVector::ZeroVector;

	// Check movement input by looking at velocity on XY plane
	FVector HorizontalVelocity = GetVelocity();
	HorizontalVelocity.Z = 0.f;

	if (!HorizontalVelocity.IsNearlyZero())
	{
		DashDir = HorizontalVelocity.GetSafeNormal();
	}
	else
	{
		DashDir = GetActorForwardVector();
	}

	DashStart = GetActorLocation();
	DashTarget = DashStart + DashDir * DashDistance;
	DashElapsedTime = 0.f;
	bIsDashing = true;

	// Enable dash collision
	DashCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWizzardCharacter::ResetDashCooldown()
{
	bCanDash = true;
	UE_LOG(LogTemp, Log, TEXT("Dash Ready!"));
}

void AWizzardCharacter::HandleDash(float DeltaTime)
{
	DashElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(DashElapsedTime / DashDuration, 0.0f, 1.0f);

	FVector NewLocation = FMath::Lerp(DashStart, DashTarget, Alpha);
	FHitResult Hit;
	SetActorLocation(NewLocation, true, &Hit);

	if (Alpha >= 1.0f)
	{
		bIsDashing = false;
		DashCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWizzardCharacter::HandleWin()
{
	GetCharacterMovement()->DisableMovement();
	HasWon = true;

	// Rotate character down
	FRotator WinRotation = FRotator(0, 180, 0);
	SetActorRotation(WinRotation);
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

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AWizzardCharacter::StartDash);
	}
}

void AWizzardCharacter::ActivateShield(int32 Hits)
{
	ShieldHitsRemaining += Hits;

	if (ShieldHitsRemaining > 0)
	{
		bHasShield = true;
		ShieldMesh->SetVisibility(true);
	}

	if (WizzardHUD)
	{
		WizzardHUD->SetShieldHits(ShieldHitsRemaining);
	}
}

void AWizzardCharacter::BreakShield()
{
	bHasShield = false;
	ShieldHitsRemaining = 0;

	UGameplayStatics::PlaySound2D(this, BreakShieldSound);
	ShieldMesh->SetVisibility(false);

	if (WizzardHUD)
	{
		WizzardHUD->SetShieldHits(ShieldHitsRemaining);
	}
}

void AWizzardCharacter::ApplyRapidFire(float Multiplier)
{
	ActiveRapidFireBuffs++;
	ProjectileCastTime = BaseProjectileCastTime / FMath::Pow(Multiplier, ActiveRapidFireBuffs);

	if (WizzardHUD)
	{
		WizzardHUD->SetRapidFireBuffs(ActiveRapidFireBuffs);
	}
}

void AWizzardCharacter::RemoveRapidFire()
{
	ActiveRapidFireBuffs--;

	if (ActiveRapidFireBuffs <= 0)
	{
		ActiveRapidFireBuffs = 0;
		ProjectileCastTime = BaseProjectileCastTime;
	}

	if (WizzardHUD)
	{
		WizzardHUD->SetRapidFireBuffs(ActiveRapidFireBuffs);
	}
}

void AWizzardCharacter::ApplySpeedBoost(float Amount)
{
	ActiveSpeedBuffs++;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed + (Amount * ActiveSpeedBuffs);

	if (WizzardHUD)
	{
		WizzardHUD->SetSpeedBuffs(ActiveSpeedBuffs);
	}
}

void AWizzardCharacter::RemoveSpeedBoost(float Amount)
{
	ActiveSpeedBuffs--;

	if (ActiveSpeedBuffs <= 0)
	{
		ActiveSpeedBuffs = 0;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed + (Amount * ActiveSpeedBuffs);
	}

	if (WizzardHUD)
	{
		WizzardHUD->SetSpeedBuffs(ActiveSpeedBuffs);
	}
}

void AWizzardCharacter::OnDashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	UE_LOG(LogTemp, Log, TEXT("Dash Overlap"));

	UGameplayStatics::ApplyDamage(OtherActor, DashDamage, GetController(), this, nullptr);
}

float AWizzardCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (bHasShield)
	{
		ShieldHitsRemaining--;

		if (ShieldHitsRemaining <= 0)
		{
			BreakShield();
		}

		return 0.0f;
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
