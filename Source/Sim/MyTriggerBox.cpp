// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTriggerBox.h"
#include "ObjectPawn.h"
#include "ConveyorBelt.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"
#include "DrawDebugHelpers.h"

AMyTriggerBox::AMyTriggerBox()
{
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::OnOverlapBegin(AActor* OverlappedComp, AActor* OtherActor)
{
}

void AMyTriggerBox::OnOverlapEnd(AActor* OverlappedComp, AActor* OtherActor)
{
    if (OtherActor != nullptr) {
        AConveyorBelt* OtherOther = Cast<AConveyorBelt>(OtherActor);
        if (OtherOther == nullptr) {
            UGameManager* GM = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
            AObjectPawn* MyDoor = Cast<AObjectPawn>(OtherActor);
            if(MyDoor)
                if (MyDoor->type == objectAdmited){
                    if (objectsLeft > 1) {
                        objectsLeft--;
                        TextLabel->GetTextRender()->SetText(FText::FromString(FString::FromInt(objectsLeft)));
                    }
                    else {
                        TextLabel->GetTextRender()->SetText(FText::FromString("Done"));
                        packageA->SetVisibilityC(true);
                        packageA->ChangeGravity(true);
                        if (objectAdmited == ETypeObjet::PACKAGE) {
                            GM->WinVictory();
                        }
                    }
                }    
        }
    }
}

void AMyTriggerBox::BeginPlay()
{
    Super::BeginPlay();

  /*  DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);*/

}
