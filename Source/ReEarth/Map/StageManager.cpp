// Fill out your copyright notice in the Description page of Project Settings.

#include "StageManager.h"
#include "Kismet\KismetSystemLibrary.h"
#include "MovieScene\Public\MovieSceneSequencePlayer.h"
#include "Enemy/Character/EnemyBase.h"
#include "TimeManagement\Public\TimeManagementBlueprintLibrary.h"

// Sets default values
AStageManager::AStageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStageManager::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < StageIndex.Num(); i++)
	{
		for (int j = 0; j < StageIndex[i].sequenceInfo.Num(); j++)
		{
			if (StageIndex[i].sequenceInfo[j].LevelSequence != nullptr)
			{
				ALevelSequenceActor* dummyActor;
				StageIndex[i].sequenceInfo[j].LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
					GetWorld(),
					StageIndex[i].sequenceInfo[j].LevelSequence,
					FMovieSceneSequencePlaybackSettings(),
					dummyActor
				);
			}
		}
	}
}

// Called every frame
void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("ss"));
	FLatentActionInfo latenInfo;
	UKismetSystemLibrary::Delay(this, 2, latenInfo);

	for (int i = 0; i < StageIndex.Num(); i++)
	{
		if (StageIndex[i].isUse)
		{
			if (!StageIndex[i].sequenceInfo[StageIndex[i].Wave].isPlay)
			{
				StageIndex[i].sequenceInfo[StageIndex[i].Wave].isPlay = true;
				StageIndex[i].sequenceInfo[StageIndex[i].Wave].LevelSequencePlayer->Play();
			}
			float endTime = UTimeManagementBlueprintLibrary::Conv_QualifiedFrameTimeToSeconds(StageIndex[i].sequenceInfo[StageIndex[i].Wave].LevelSequencePlayer->GetEndTime());
			UKismetSystemLibrary::Delay(this, endTime, latenInfo);
			StageIndex[i].Wave++;
			if (StageIndex[i].Wave == StageIndex[i].sequenceInfo.Num())
			{
				StageIndex[i].isUse = false;
			}
		}
	}
}

void AStageManager::SetterUse(int num)
{
	StageIndex[num].isUse = true;
}

void AStageManager::AddEnemy(AEnemyBase * enemy)
{
	if (nullptr != enemy)
	{
		UE_LOG(LogTemp, Log, TEXT("AStageManager :: AddEnemy"));
		EnemyArray.Add(enemy);
	}
}