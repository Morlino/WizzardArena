// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FireProjectile.h"

#include "WizzardCharacter.h"

void UAnimNotify_FireProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AWizzardCharacter* Character = Cast<AWizzardCharacter>(MeshComp->GetOwner()))
	{
		Character->ShootProjectile();
	}
}
