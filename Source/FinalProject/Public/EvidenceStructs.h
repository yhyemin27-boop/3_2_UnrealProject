// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" //FTableRowBase 사용을 위해 필수
#include "EvidenceStructs.generated.h"

// 모든 증거 아이템이 가질 데이터 구조 정의
USTRUCT(BlueprintType)
struct FEvidenceData : public FTableRowBase
{
    GENERATED_BODY()

    // Data Table에서 사용할 고유 ID (Row Name과 일치시킬 예정)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName EvidenceID;

    // 인벤토리에 표시될 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    // 상세 설명
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
    FText DescriptionText;

    // 인벤토리에 표시될 이미지 (Blueprint에서 지정)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UTexture2D* IconImage;

};