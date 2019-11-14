// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageStruct.h"
#include "StageManager.generated.h"

UCLASS()
class REEARTH_API AStageManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�������� �迭
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FStageStruct> StageIndex;
	
	//������ ���� ���� �� ���� ����
	UFUNCTION(BlueprintCallable)
	void AddEnemy(AEnemyBase* enemy);
	
	//�������� ����
	//-----------------------------
	UFUNCTION(BlueprintCallable)
	void StartStage(int num);
	void PlayStage();
	void EndStage();
	int Wave = 0;
	int CurrentStage;
	//-----------------------------

private:
	//������ ��
	UPROPERTY()
	TArray<AEnemyBase*> EnemyArray;

	//������ ������ �ð�
	FTimerHandle SequenceEndTimerHandle;

	//���̺� ���� ����ִ� �� Ȯ��
	FTimerHandle CheckAliveEnemyHandle;

	AActor* SpawnedBarrier;
};