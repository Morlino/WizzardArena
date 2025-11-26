// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EnemyAttack.h"
#include "EnemyCharacter.h"

void UAnimNotify_EnemyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AEnemyCharacter* Character = Cast<AEnemyCharacter>(MeshComp->GetOwner()))
	{
		Character->TraceAttack(TraceRadius, TraceOffset);
	}
}
