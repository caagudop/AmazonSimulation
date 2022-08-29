// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameManager.h"
#include "Engine/TextRenderActor.h"
#include "MyTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class SIM_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// constructor sets default values for this actor's properties
	AMyTriggerBox();

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedComp, class AActor* OtherActor);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedComp, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	ETypeObjet objectAdmited;

	UPROPERTY(EditAnywhere)
	class AObjectPawn* packageA;

	UPROPERTY(EditAnywhere)
		ATextRenderActor* TextLabel;

	UPROPERTY(EditAnywhere)
		float objectsLeft = 2;
};
