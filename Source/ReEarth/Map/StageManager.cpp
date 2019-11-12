// Fill out your copyright notice in the Description page of Project Settings.

#include "StageManager.h"
#include "Kismet\KismetSystemLibrary.h"
#include "MovieScene\Public\MovieSceneSequencePlayer.h"
#include "Enemy/Character/EnemyBase.h"
#include "TimeManagement\Public\TimeManagementBlueprintLibrary.h"
#include "TimerManager.h"
#include "Map\StageBarrier.h"

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
}

void AStageManager::AddEnemy(AEnemyBase * enemy)
{
	if (nullptr != enemy)
	{
		UE_LOG(LogTemp, Log, TEXT("AStageManager :: AddEnemy"));
		EnemyArray.Add(enemy);
	}
}

void AStageManager::StartStage(int num)
{
	UE_LOG(LogTemp, Log, TEXT("AStageManager :: StartStage"));

	CurrentStage = num;

	SpawnedBarrier = GetWorld()->SpawnActor<AActor>(StageIndex[CurrentStage].Barrier, StageIndex[CurrentStage].BarrierTransform);
	Cast<AStageBarrier>(SpawnedBarrier)->BarrierSize = StageIndex[CurrentStage].BarrierSize;
	Cast<AStageBarrier>(SpawnedBarrier)->CreateBarrier();

	if (Wave == 0)
	{
		for (int i = 0; i < EnemyArray.Num(); i++)
		{
			Destroy(EnemyArray[i]);
		}
	}

	if (!StageIndex[CurrentStage].sequenceInfo[Wave].isPlay)
	{
		StageIndex[CurrentStage].sequenceInfo[Wave].isPlay = true;
		StageIndex[CurrentStage].sequenceInfo[Wave].LevelSequencePlayer->Play();
	}

	float endTime = UTimeManagementBlueprintLibrary::Conv_QualifiedFrameTimeToSeconds(StageIndex[CurrentStage].sequenceInfo[Wave].LevelSequencePlayer->GetEndTime());
	GetWorldTimerManager().SetTimer(SequenceEndTimerHandle, this, &AStageManager::PlayStage, endTime, false);
}

void AStageManager::PlayStage()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManager :: PlayStage"));

	bool EnemyStillAlive = false;
	for (int i = 0; i < EnemyArray.Num(); i++)
	{
		if (!EnemyArray[i]->IsDead)
		{
			EnemyStillAlive = true;
			break;
		}
	}

	if (!EnemyStillAlive)
	{
		Wave++;
		//Check Remain Stage
		if (Wave == StageIndex[CurrentStage].sequenceInfo.Num())
			EndStage();
		else
			StartStage(CurrentStage);
	}
	else
		GetWorldTimerManager().SetTimer(CheckAliveEnemyHandle, this, &AStageManager::PlayStage, 2, false);
}

void AStageManager::EndStage()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManager :: EndStage"));

	StageIndex[CurrentStage].isUse = true;	
	Cast<AStageBarrier>(SpawnedBarrier)->SelfDestroy();
	Wave = 0;
	CurrentStage = 0;
}