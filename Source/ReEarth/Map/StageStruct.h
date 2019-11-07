// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SequenceInfoStruct.h"
#include "StageStruct.generated.h"

USTRUCT(BlueprintType)
struct FStageStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSequenceInfoStruct> sequenceInfo;



	bool isUse = false;
	int Wave = 0;
};