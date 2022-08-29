// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "ObjectPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


ASimCharacter::ASimCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 34.f)); 
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	PrimaryActorTick.bCanEverTick = true;
	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->AddRelativeLocation(FVector(50.f, 20.f, 10.f));
	HoldingComponent->SetupAttachment(FP_MuzzleLocation);

	CurrentItem = NULL;
	bCanMove = true;
	bInspecting = false;
}

void ASimCharacter::BeginPlay()
{
	Super::BeginPlay();
	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;
}


void ASimCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ASimCharacter::OnAction);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &ASimCharacter::OnInspect);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &ASimCharacter::RestartLevel);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &ASimCharacter::OnInspectReleased);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASimCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASimCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASimCharacter::LookUpAtRate);
}

void ASimCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Start = FirstPersonCameraComponent->GetComponentLocation();
	ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	End = ((ForwardVector * 200.f) + Start);

	if (!bHoldingItem)
	{
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
		{
			if (Hit.GetActor()->GetClass()->IsChildOf(AObjectPawn::StaticClass()))
			{
				CurrentItem = Cast<AObjectPawn>(Hit.GetActor());
			}
		}
		else
		{
			CurrentItem = NULL;
		}
	}

	if (bInspecting)
	{
		if (bHoldingItem)
		{
			FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));
			HoldingComponent->SetRelativeLocation(InspectVector);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.9000002f;
			CurrentItem->RotateActor();
		}
	}
	else
	{
		FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));

		if (bHoldingItem)
		{
			HoldingComponent->SetRelativeLocation(FVector(00.0f, 00.0f, 0.0f));
		}
	}
}

void ASimCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && bCanMove)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASimCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && bCanMove)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASimCharacter::OnAction()
{
	if (CurrentItem && !bInspecting)
	{
		ToggleItemPickup();
	}
}

void ASimCharacter::OnInspect()
{
	if (bHoldingItem)
	{
		LastRotation = GetControlRotation();
		ToggleMovement();
	}
	else
	{
		bInspecting = true;
	}
}

void ASimCharacter::OnInspectReleased()
{
	if (bInspecting && bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	}
	else
	{
		bInspecting = false;
	}
}

void ASimCharacter::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	FirstPersonCameraComponent->bUsePawnControlRotation = !FirstPersonCameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void ASimCharacter::ToggleItemPickup()
{
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->Pickup();

		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
}

void ASimCharacter::RestartLevel()
{
	UGameManager* GM = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	GM->RestartLevel();
}

void ASimCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

