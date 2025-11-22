// Fill out your copyright notice in the Description page of Project Settings.


#include "WizzardCharacter.h"

#include "NavigationSystemTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "WizzardPlayerController.h"

// Sets default values
AWizzardCharacter::AWizzardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 800.0f;
	SpringArmComp->bDoCollisionTest = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void AWizzardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	if (!PC) 
	{
		UE_LOG(LogTemp, Warning, TEXT("RotateToCursor: No PlayerController!"));
		return;
	}

	FVector WorldLocation, WorldDirection;
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector PlaneIntersection = FMath::LinePlaneIntersection(
			WorldLocation,
			WorldLocation + WorldDirection * 1000.0f,
			FVector(0.0f, 0.0f, GetActorLocation().Z),
			FVector::UpVector
		);

		FVector LookDir = PlaneIntersection - GetActorLocation();
		LookDir.Z = 0.0f;

		UE_LOG(LogTemp, Log, TEXT("Cursor WorldLocation: %s, Intersection: %s, LookDir: %s"), 
			*WorldLocation.ToString(), 
			*PlaneIntersection.ToString(), 
			*LookDir.ToString());

		if (!LookDir.IsNearlyZero())
		{
			FRotator TargetRotation = LookDir.Rotation();
			TargetRotation.Pitch = 0.f;
			TargetRotation.Roll = 0.f;
			FRotator MeshOffset(MeshRotationOffset);
			GetMesh()->SetWorldRotation(TargetRotation + MeshOffset);

			UE_LOG(LogTemp, Log, TEXT("Rotating to: %s"), *TargetRotation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LookDir is zero, skipping rotation"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeprojectMousePositionToWorld failed"));
	}
}

// Called every frame
void AWizzardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToCursor();
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
	}
}

