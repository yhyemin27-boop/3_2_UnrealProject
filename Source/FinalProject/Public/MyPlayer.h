//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EvidenceStructs.h" // FEvidenceData 구조체 사용을 위해 필수
#include "My_GameInstance.h"

#include "BaseNPC.h"//MPC 클래스

#include "MyPlayer.generated.h"

UCLASS()
class FINALPROJECT_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Component
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* CamComp;

	// Enhanced Input
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* imcDefault;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* imcMouseLook;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaMove;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaMouseLook;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaInventory;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaNote;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaFound;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaPropose;

	UPROPERTY(EditDefaultsOnly, Category = "Evidence")
	class UDataTable* EvidenceDataTable;

	// --------------------------------------------------------
	// --- Evidence Data & Utility Functions ---
	// --------------------------------------------------------

	// 증거 목록 배열 (블루프린트 접근 가능)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evidence")
	TArray<FEvidenceData> EvidenceList;

	// 배열에 증거를 추가하고 인덱스를 반환하는 함수 (블루프린트 호출 가능)
	UFUNCTION(BlueprintCallable, Category = "Evidence")
	int32 AddEvidenceToEvidenceList(const FEvidenceData& EvidenceData);

	// --------------------------------------------------------
	// --- Blueprint Events (Lowercase names for BP Bridge) ---
	// --------------------------------------------------------

	// 증거 획득 시 블루프린트로 데이터 전달
	UFUNCTION(BlueprintImplementableEvent, Category = "Evidence")
	void OnEvidenceFound(const FEvidenceData& EvidenceData);

	// 인벤토리 UI 토글 로직을 처리할 블루프린트 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Input|UI")
	void toggleInventory();

	// 노트/수첩 UI 토글 로직을 처리할 블루프린트 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Input|UI")
	void toggleNote();

	// 증거 제출 UI 로직을 처리할 블루프린트 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Input|UI")
	void proposeEvidence();

	// --------------------------------------------------------
	// --- Input Functions (Uppercase names for C++ Input Handlers) ---
	// --------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaCheating;

	// --------------------------------------------------------
	// --- Input Functions (Uppercase names for C++ Input Handlers) ---
	// --------------------------------------------------------

	void Move(const struct FInputActionValue& inputValue);
	void Look(const struct FInputActionValue& inputValue);

	void ToggleInventory(const struct FInputActionValue& inputValue);//(증거 인벤토리)
	void ToggleNote(const struct FInputActionValue& inputValue);//노트 열기(그 조작키 들어간거
	void Found(const struct FInputActionValue& inputValue); //찾기
	void ProposeEvidence(const struct FInputActionValue& inputValue); //증거 제출
	void AddEvidenceToInventory(class AEvidenceActor* Evidence); //한번 회득 증거 다시 노 획득핟게

	//치팅 관련 함수
	void AdvanceDayCheat(const struct FInputActionValue& inputValue);
};