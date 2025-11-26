// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API UAnimNotify_EnemyAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	// Trace radius for this attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float TraceRadius = 50.f;

	// Forward offset for the trace (relative to mesh)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector TraceOffset = FVector::ZeroVector;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
