// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

//------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle = 0		UMETA(DisplayName = "Idle"),
	Move = 1		UMETA(DisplayName = "Move"),
	Attack = 2		UMETA(DisplayName = "Attack"),
	Dead = 3		UMETA(DisplayName = "Dead")
};

//------------------------------------------------------------------------------------
UCLASS()
class REEARTH_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()
			   
public:

	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void AddEnemyToRobot();

	UFUNCTION(BlueprintCallable)
	void AddEnemyToManager();
	
	void RemoveEnemy();

	//------------------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class APlayerPawn* AttackTarget;
	   	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EEnemyState State;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentHP = 100;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool EnableMove = true;
	   
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsVisibleAttackTarget = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent * HomingPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent * HomingWidget;

	//------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void ChangeState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable)
	bool CheckState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable)
	AActor* GetDestinationTarget();

	UFUNCTION(BlueprintCallable)
	virtual bool GetVisibleAttackTarget();

	UFUNCTION(BlueprintCallable)
	virtual float GetAttackTargetRot();
	
	UPhysicsAsset* MeshPhsicsDeadAsset;
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
		
	//------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackStart();
	virtual void AttackStart_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackEnd();
	virtual void AttackEnd_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeadEnd();
	virtual void DeadEnd_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RotateAttactTargetLoc();
	virtual void RotateAttactTargetLoc_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAIControllerRun(bool IsRun);
	virtual void SetAIControllerRun_Implementation(bool IsRun);
	
	//HomingOn함수
	//Player로부터 유도미사일 Target으로 지정됐을 때 해야할 작업
	void HomingOn();
	//--------------------------------------------------------
	void HomingOff();

	FTimerHandle HomingOffHandle;
	   
	//------------------------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsSpawned = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsAIControllerRun = true;

	void ManagerCreated();
	void RobotCreated();
	
};
