// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "BaseNPC.generated.h"

UCLASS()

class FINALPROJECT_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()


public:
	// 생성자
	ABaseNPC();

protected:
	virtual void BeginPlay() override;



public:
	//NPC 고유아디
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Info")

	FName NPC_ID;
	//NPC가 플레이어와 상호작용할 때 호출되는 함수 (대화 시작)

	// 블루프린트에서 호출
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void InitiateDialogue();

	//현재 NPC가 플레이어에게 보여줄 대화 텍스트
	// 데이터 테이블 검색 결과
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText CurrentDialogueText;

	///대화 시작 시 NPC 블루프린트의 Event Graph를 실행
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void OnDialogueInitiated(const FText& DialogueText);

};