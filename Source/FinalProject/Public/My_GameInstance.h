// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h" // UDataTable 사용을 위해 추가
#include "Finalproject/public/DayNPCData.h"
#include "My_GameInstance.generated.h"

UCLASS()
class FINALPROJECT_API UMy_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
   
    // 초기값을 0으로 설정합니다. (Day 0 시작)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Flow")
    int32 CurrentDay = 0; //그그 사건 현장 그부분을 0

    // 다음 날 진행 및 TP 함수
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void AdvanceToNextDayAndTeleport();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Flow")
    FName CurrentNPCID;

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void TeleportToInterrogationRoom();//취조실 이동 함수
    // 1. NPCSpawnTable 선언
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Data")
    class UDataTable* NPCSpawnTable;

    // 2. CurrentSpawnedNPC 선언
    UPROPERTY()
    ACharacter* CurrentSpawnedNPC = nullptr;

    UFUNCTION(BlueprintCallable, Category = "NPC Management")
    void ChangeNPCBasedOnDay(int32 Day);//NPC를 교체하는 함수

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Data")
    class UDataTable* DialogueResponseDataTable; //대화 응답 데이터 테이블 변수

private:
    // 날짜별 TP 위치를 반환하는 함수
    FVector GetTeleportLocationForDay(int32 Day);
};