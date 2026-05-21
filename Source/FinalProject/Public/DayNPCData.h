#pragma once
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase의 정의가 있는 헤더
#include "Finalproject/Public/DialogueResponseData.h"
#include "DayNPCData.generated.h" 

// USTRUCT로 정의하며, FTableRowBase를 상속받습니다.
USTRUCT(BlueprintType)
struct FDayNPCData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Spawn")
    TSubclassOf<class ACharacter> NPC_Class;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Spawn")
    FVector SpawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Spawn")
    FRotator SpawnRotation;

};