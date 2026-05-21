// Fill out your copyright notice in the Description page of Project Settings.


#include "My_GameInstance.h"
#include "finalproject/Public/BaseNPC.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 사용 (컨트롤러/폰 가져오기)
#include "GameFramework/PlayerController.h" // APlayerController 사용
#include "GameFramework/Pawn.h" // APawn 사용 (SetActorLocation 함수 호출)

FVector UMy_GameInstance::GetTeleportLocationForDay(int32 Day)
{
	if (Day == 0)
	{
		// Day 0: 게임시작 위치-> 나중에 현관에서 위젯 Trggierbox로 한번만 보여주기(나중에 추가할예정)
		return FVector(-2520.0f, 960.0f, 90.0f);
	}
	else if (Day == 1)
	{
		// Day 1: 첫 번째 조사 장소 (집 A) 
		return FVector(2330.0f, 1700.0f, 150.0f);
	}
	else if (Day == 2)
	{
		// Day 2: 두 번째 조사 장소 (집 B)
		return FVector(-2820.0f, -3060.0f, 130.0f);
	}
	else if (Day == 3)
	{
		// Day 3: 세 번째 조사 장소 (집 C)
		return FVector(-360.0f, 960.0f, 120.0f);
	}

	// Day 4 이상 -> 나중에 엔딩 위젯 추가
	return FVector::ZeroVector;
}

void UMy_GameInstance::AdvanceToNextDayAndTeleport()
{
	// 1. 날짜 증가
	CurrentDay++;

	ChangeNPCBasedOnDay(CurrentDay);//MPC교체

	// 2. 증가된 날짜에 해당하는 TP 위치를 가져옵니다.
	FVector NewLocation = GetTeleportLocationForDay(CurrentDay);

	// 3. 플레이어 TP
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (APawn* PlayerPawn = PC->GetPawn())
		{
			PlayerPawn->SetActorLocation(NewLocation);
			UE_LOG(LogTemp, Warning, TEXT("다음날:Day%d"), CurrentDay);
		}
	}
}

void UMy_GameInstance::ChangeNPCBasedOnDay(int32 Day)
{
	if (!NPCSpawnTable) {
		UE_LOG(LogTemp, Error, TEXT("NPCSpawnTable이 설정되지 않았습니다!"));
		return;
	}
	//이전 NPC 파괴
	if (CurrentSpawnedNPC && CurrentSpawnedNPC->IsValidLowLevel())
	{
		CurrentSpawnedNPC->Destroy();
		CurrentSpawnedNPC = nullptr;
	}
	// 2. 데이터 테이블에서 Row Name으로 데이터 조회
	FName RowName = *FString::Printf(TEXT("Day_%d"), Day);
	FDayNPCData* NPCData = NPCSpawnTable->FindRow<FDayNPCData>(RowName, TEXT("Find NPC Data for Day"));

	if (NPCData && NPCData->NPC_Class) //List에서 호출완료 했으나 ID .오류 해결하면 지우기
	{
		if (Day == 1)		{
			CurrentNPCID = "suspect_A";
		}
		else if (Day == 2)		{
			CurrentNPCID = "suspect_B"; // B가 호출되도록 수정
		}
		else if (Day == 3)		{
			CurrentNPCID = "suspect_C"; // C가 호출되도록 수정
		}
		if (NPCData && NPCData->NPC_Class)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				// 3. 새로운 NPC 스폰
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				ACharacter* NewNPC = World->SpawnActor<ACharacter>(
					NPCData->NPC_Class,
					NPCData->SpawnLocation,
					NPCData->SpawnRotation,
					SpawnParams
				);
				if (NewNPC) CurrentSpawnedNPC = NewNPC;
			}
		}
	}
}

void UMy_GameInstance::TeleportToInterrogationRoom()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn) return;

	// 취조실 좌표
	FVector InterrogationLocation(-2520.0f, 960.0f, 90.0f);
	FRotator NewRotation(0.0f, 0.0f, 0.0f);

	// 텔레포트 실행
	PlayerPawn->SetActorLocationAndRotation(InterrogationLocation, NewRotation, false, nullptr, ETeleportType::TeleportPhysics);

}
