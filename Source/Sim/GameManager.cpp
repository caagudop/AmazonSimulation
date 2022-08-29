// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "Kismet/GameplayStatics.h"

UGameManager::UGameManager()
{
}

void UGameManager::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void UGameManager::WinVictory()
{
	myDelegate.Broadcast();
}
