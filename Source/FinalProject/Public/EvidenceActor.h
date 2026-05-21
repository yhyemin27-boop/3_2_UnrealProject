// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EvidenceActor.generated.h"

UCLASS()
class FINALPROJECT_API AEvidenceActor : public AActor //증거 클래스~
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEvidenceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UStaticMeshComponent* EvidenceMesh;
	//Static 매쉬 선언

	UPROPERTY(EditAnywhere, Category = "Evidence Data")
	FName EvidenceDataKey;

	// 획득 플래그는 그대로 유지
	UPROPERTY(VisibleAnywhere, Category = "Evidence Data")
	bool bIsFound = false;
};
