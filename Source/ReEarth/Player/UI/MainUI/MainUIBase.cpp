// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBase.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UMainUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	RobotHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar")));
	
	for (int iCnt = 0; iCnt < df_ALL_MISSILENUM; ++iCnt)
	{
		//Missile_1,Missile_2,Missile_3,Missile_4 만들어서 각 배열에 Attach
		FString MissileName = FString("Missile_").Append(FString::FromInt(iCnt+1));
		Missile.Add(Cast<UImage>(GetWidgetFromName(FName(*MissileName))));
	}

	//초기에는 꽉 차있는 상태
	SetHPBar();
}

void UMainUIBase::SetHPBar(float CurrentHP_Rate)
{
	RobotHPBar->SetPercent(CurrentHP_Rate);
}

void UMainUIBase::SetMissileVisibility(ESlateVisibility NewState, int iIdx)
{
	Missile[iIdx]->SetVisibility(NewState);
}

void UMainUIBase::UpdateRemoveMissile()
{
	SetMissileVisibility(ESlateVisibility::Hidden, CurrentMissileMaxIdx);
	--CurrentMissileMaxIdx;
}

void UMainUIBase::UpdateAddMissile()
{
	++CurrentMissileMaxIdx;
	SetMissileVisibility(ESlateVisibility::Visible, CurrentMissileMaxIdx);
}
