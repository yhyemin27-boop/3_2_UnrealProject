// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h" //이 헤더가 있는지 확인

// Sets default values
AEvidenceActor::AEvidenceActor()
{
    // Actor Tick은 필요 없으므로 비활성화
    PrimaryActorTick.bCanEverTick = false;

    // Static Mesh Component 생성 및 루트 컴포넌트로 설정
    EvidenceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EvidenceMesh"));
    RootComponent = EvidenceMesh;

    //물리 충돌 제거: 증거가 플레이어에게 밀리지 않도록 NoCollision 설정
    EvidenceMesh->SetCollisionProfileName(TEXT("NoCollision"));

    EvidenceMesh->SetCollisionResponseToChannel( //Block설정
        ECollisionChannel::ECC_Visibility,
        ECollisionResponse::ECR_Block
    );

}

// Called when the game starts or when spawned
void AEvidenceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEvidenceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

