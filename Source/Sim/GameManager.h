// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyDelegate);
/**
 * 
 */
UENUM(BlueprintType)
enum class ETypeObjet : uint8
{
	DESK,
	STATUE,
	CHAIR,
	PACKAGE,
	OTHERS
};
UCLASS()
class SIM_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameManager();

	void RestartLevel();
	void WinVictory();


	UPROPERTY(BlueprintAssignable)
	FMyDelegate myDelegate;

	
};
