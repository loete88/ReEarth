// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StepSound.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"


bool UAnimNotify_StepSound::Received_Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Received_Notify(MeshComp, Animation);

	FVector SoundLocation = MeshComp->GetSocketLocation(TEXT("FootSoundPosition"));

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, SoundLocation);

	return true;
}
