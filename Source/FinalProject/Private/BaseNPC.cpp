// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNPC.h"
#include "My_GameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	// 기본 아이디 ->에디터에서 개별적으로 변경
	NPC_ID = FName(TEXT("Default_NPC_ID"));
}

void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseNPC::InitiateDialogue()
{// 1. UMy_GameInstance를 가져옵니다.
	UMy_GameInstance* GameInstance = Cast<UMy_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("InitiateDialogue: UMy_GameInstance를 찾을 수 없습니다!"));
		return;
	}

	// 2. DialogueResponseDataTable 유효성 검사 (Game Instance에서 연결되어 있는지 확인)
	if (!GameInstance->DialogueResponseDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("InitiateDialogue: DialogueResponseDataTable이 GameInstance에 설정되지 않았습니다!"));
		CurrentDialogueText = FText::FromString(TEXT("오류: 데이터 테이블이 연결되지 않음."));
		return;
	}

	// 3. Row Name 생성: "Day_X_NPCID" 형식
	FString RowNameStr = FString::Printf(TEXT("Day_%d_%s"), GameInstance->CurrentDay, *NPC_ID.ToString());
	FName RowName = *RowNameStr;

	// 4. 데이터 테이블 검색
	FDialogueResponseData* DialogueData = GameInstance->DialogueResponseDataTable->FindRow<FDialogueResponseData>(RowName, TEXT("Find Dialogue Response"));

	if (DialogueData)
	{
		CurrentDialogueText = DialogueData->DialogueText;
	}
	else
	{
		CurrentDialogueText = FText::FromString(TEXT("데이터 테이블에서 MPC의 대사를 찾지 못했습니다."));
		UE_LOG(LogTemp, Warning, TEXT("NPC %s: Row Name %s 에 해당하는 대화 데이터를 찾을 수 없습니다."), *NPC_ID.ToString(), *RowNameStr);
	}

	// 블루프린트 이벤트를 호출
	OnDialogueInitiated(CurrentDialogueText);

	// 디버그 로그
	UE_LOG(LogTemp, Warning, TEXT("NPC %s: Dialogue Initiated. Text: %s"), *NPC_ID.ToString(), *CurrentDialogueText.ToString());
}