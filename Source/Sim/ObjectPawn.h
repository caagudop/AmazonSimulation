// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "ObjectPawn.generated.h"

UCLASS()
class AObjectPawn : public AActor
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AObjectPawn();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* HoldingComp;

	UFUNCTION()
		void RotateActor();

	UFUNCTION()
		void Pickup();
	UPROPERTY(EditAnywhere)
		ETypeObjet type;

	bool bHolding;

	UPROPERTY(EditAnywhere)
	bool bGravity = true;

	UPROPERTY(EditAnywhere)
	bool bVisibility = true;

	void SetVisibilityC(bool visible);
	void ChangeGravity(bool gravity);

	FRotator ControlRotation;
	ACharacter* MyCharacter;
	UCameraComponent* PlayerCamera;
	FVector ForwardVector;
};
