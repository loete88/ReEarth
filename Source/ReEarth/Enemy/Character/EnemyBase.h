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

protected:

	virtual void BeginPlay() override;
	void AddEnemy();

public:
	   
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

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void MovePatrolLoc();
	//virtual void MovePatrolLoc_Implementation();

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void MoveAttackTarget();
	//virtual void MoveAttackTarget_Implementation();

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void MoveDestinationTarget(AActor* Target);
	//virtual void MoveDestinationTarget_Implementation(AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RotateAttactTargetLoc();
	virtual void RotateAttactTargetLoc_Implementation();
	


	//HomingOn�Լ�
	//Player�κ��� �����̻��� Target���� �������� �� �ؾ��� �۾�
	void HomingOn();
	//--------------------------------------------------------
	
	void HomingOff();

	FTimerHandle HomingOffHandle;



	//------------------------------------------------------------------------------------
};
