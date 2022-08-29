// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectPawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AObjectPawn::AObjectPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	MyMesh->SetSimulatePhysics(true);
	RootComponent = MyMesh;

	bHolding = false;
	bGravity = true;
	
}

// Called when the game starts or when spawned
void AObjectPawn::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;

	MyCharacter->GetComponents(Components);

	if (Components.Num() > 0)
	{
		for (auto& Comp : Components)
		{
			if (Comp->GetName() == "HoldingComponent")
			{
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
	if (!bVisibility){
		MyMesh->SetVisibility(bVisibility);
	}
	if (!bGravity){
		MyMesh->SetEnableGravity(bGravity);
		MyMesh->SetSimulatePhysics(bGravity);
	}
}

// Called every frame
void AObjectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding && HoldingComp)
	{
		SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
	}
}

void AObjectPawn::RotateActor()
{
	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(ControlRotation));
}

void AObjectPawn::Pickup()
{
	bHolding = !bHolding;
	bGravity = !bGravity;
	MyMesh->SetEnableGravity(bGravity);
	MyMesh->SetSimulatePhysics(bHolding ? false : true);
	MyMesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (!bHolding)
	{
		ForwardVector = PlayerCamera->GetForwardVector();
		MyMesh->AddForce(ForwardVector * 10000 * MyMesh->GetMass());
	}
}

void AObjectPawn::SetVisibilityC(bool visible)
{
	bVisibility = visible;
	MyMesh->SetVisibility(visible);
}

void AObjectPawn::ChangeGravity(bool gravity)
{
	bGravity = gravity;
	MyMesh->SetEnableGravity(gravity);
	MyMesh->SetSimulatePhysics(gravity);
}

