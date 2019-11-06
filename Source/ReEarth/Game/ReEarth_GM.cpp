// Fill out your copyright notice in the Description page of Project Settings.


#include "ReEarth_GM.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Engine/Classes/Kismet/StereoLayerFunctionLibrary.h"

void AReEarth_GM::BeginPlay()
{
	//Loading 화면 설정하기
	//----------------------------------------------------------------
	UStereoLayerFunctionLibrary::EnableAutoLoadingSplashScreen(true);
	UStereoLayerFunctionLibrary::SetSplashScreen(LoadTexture);
	UStereoLayerFunctionLibrary::HideSplashScreen();
	//----------------------------------------------------------------
}

void AReEarth_GM::GameStart()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void AReEarth_GM::GameExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		EQuitPreference::Quit, false);
}
