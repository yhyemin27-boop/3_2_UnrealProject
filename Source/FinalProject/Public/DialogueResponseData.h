// FDialogueResponseData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueResponseData.generated.h"

USTRUCT(BlueprintType)
struct FDialogueResponseData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Condition")
    FName Target_NPC_ID; // 대상 NPC ID (BaseNPC의 ID와 일치)

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Condition")
    int32 Target_Day; // 대상 날짜

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Condition")
    FName Required_Evidence_ID; // 이 대사를 유발하는 데 필요한 증거 ID

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FText DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Response", meta = (MultiLine = true))
    FText Success_Dialogue; // 증거 제시 성공 시 NPC의 대사

};