// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyType3.h"

AEnemyType3::AEnemyType3()
{
	EnableMove = false;
}

bool AEnemyType3::GetVisibleAttackTarget()
{
	return true;
}