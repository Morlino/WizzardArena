// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AI_ShootProjectiles.generated.h"

/**
 * 
 */
UCLASS()
class WIZZARDARENA_API UAnimNotify_AI_ShootProjectiles : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	            const FAnimNotifyEventReference& EventReference);
};
