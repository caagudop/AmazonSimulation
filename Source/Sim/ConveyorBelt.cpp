// Fill out your copyright notice in the Description page of Project Settings.

#include "ConveyorBelt.h"
#include "Components/BoxComponent.h"


// Sets default values
AConveyorBelt::AConveyorBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cinta"));
	MyMesh->SetupAttachment(RootComponent);

	Trigger1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Move"));
	Trigger1->SetupAttachment(MyMesh);

}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isOn){
		float movement = DeltaTime * speed;
		FVector result = MyMesh->GetForwardVector() * movement;
		TArray<AActor*> actors;
		Trigger1->GetOverlappingActors(actors);
		int num = actors.Num();
		if(num>0){
			for (int i = 0; i < num; i++)
			{
				actors[i]->AddActorWorldOffset(result);
			}
		}
	}
}

