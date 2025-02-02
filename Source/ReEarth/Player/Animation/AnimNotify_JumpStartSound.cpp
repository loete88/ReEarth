// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_JumpStartSound.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

bool UAnimNotify_JumpStartSound::Received_Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{

	Super::Received_Notify(MeshComp, Animation);

	FVector SoundLocation = MeshComp->GetComponentLocation();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, SoundLocation);

	return true;
}
