// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AI_ShootProjectiles.h"
#include "EnemyCharacter.h"

void UAnimNotify_AI_ShootProjectiles::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		Enemy->ShootProjectiles();
	}
}
