// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "AnimNotify_StepSound.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UAnimNotify_StepSound : public UAnimNotify_PlaySound
{
	GENERATED_BODY()

	bool Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
