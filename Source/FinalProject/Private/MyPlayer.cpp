// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
//증거 관련
#include "EvidenceActor.h"
#include "EvidenceStructs.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h" // UDataTable 사용을 위해 추가
#include "DrawDebugHelpers.h"
//치팅
#include "My_GameInstance.h"

// Sets default values
AMyPlayer::AMyPlayer()
{

	// 2. Camera Component 설정 (1인칭 시점)
	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(RootComponent);
	CamComp->SetRelativeLocation(FVector(0.0f, 0, 75.0f));
	CamComp->bUsePawnControlRotation = true;

	//로컬 좌표
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 캐릭터 이동 설정 
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc != nullptr) {
		pc->PlayerCameraManager->ViewPitchMin = -75.0f;//시야 최대치
		pc->PlayerCameraManager->ViewPitchMax = 75.0f;

		// Enhanced Input Subsystem에 매핑 컨텍스트 추가
		UEnhancedInputLocalPlayerSubsystem* subsys =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr) {
			subsys->AddMappingContext(imcDefault, 0);
			subsys->AddMappingContext(imcMouseLook, 0);

			UE_LOG(LogTemp, Warning, TEXT("로그 1: Mapping Contexts added successfully. (imcDefault, imcMouseLook)"));
		}
	}
}

// --- [추가 3: EvidenceList에 데이터를 추가하고 인덱스를 반환하는 함수 구현] ---
int32 AMyPlayer::AddEvidenceToEvidenceList(const FEvidenceData& EvidenceData)
{
	// TArray::Add 함수는 새로 추가된 요소의 인덱스를 반환합니다.
	int32 NewIndex = EvidenceList.Add(EvidenceData);

	//C++ 배열에 증거 추가 및 인덱스 반환 확인
	UE_LOG(LogTemp, Warning, TEXT("로그 11.5: C++ EvidenceList에 증거 추가 완료. 새 인덱스: %d. 현재 증거 개수: %d"), NewIndex, EvidenceList.Num());

	return NewIndex;
}
// --------------------------------------------------------


// Called every frame 
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input Component 바인딩
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(iaMove, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		enhancedInputComponent->BindAction(iaMouseLook, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
		enhancedInputComponent->BindAction(iaInventory, ETriggerEvent::Triggered, this, &AMyPlayer::ToggleInventory);
		enhancedInputComponent->BindAction(iaNote, ETriggerEvent::Triggered, this, &AMyPlayer::ToggleNote);


		// IA_Found 바인딩 (Started 이벤트)
		enhancedInputComponent->BindAction(iaFound, ETriggerEvent::Started, this, &AMyPlayer::Found);
		UE_LOG(LogTemp, Warning, TEXT("로그 2: IA_Found 입력 액션 바인딩 완료."));

		enhancedInputComponent->BindAction(iaPropose, ETriggerEvent::Triggered, this, &AMyPlayer::ProposeEvidence);
		//치팅 관련 Action
		enhancedInputComponent->BindAction(iaCheating, ETriggerEvent::Triggered, this, &AMyPlayer::AdvanceDayCheat);

	}
}

void AMyPlayer::Move(const struct FInputActionValue& inputValue)
{
	const FVector2D Value = inputValue.Get<FVector2D>();
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	if (!FMath::IsNearlyZero(Value.Y))
	{
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, Value.Y);
	}

	if (!FMath::IsNearlyZero(Value.X))
	{
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, Value.X);
	}
}

void AMyPlayer::Look(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	AddControllerYawInput(value.X);
	AddControllerPitchInput(value.Y);
}

void AMyPlayer::ToggleInventory(const struct FInputActionValue& inputValue)
{
	if (inputValue.Get<bool>())
	{
		toggleInventory();
	}
}

void AMyPlayer::ToggleNote(const struct FInputActionValue& inputValue)
{
	if (inputValue.Get<bool>())
	{
		toggleNote();
	}
}

void AMyPlayer::ProposeEvidence(const struct FInputActionValue& inputValue)
{
	if (inputValue.Get<bool>())
	{
		proposeEvidence();
	}
}

void AMyPlayer::Found(const struct FInputActionValue& inputValue)
{
	//키입력
	UE_LOG(LogTemp, Warning, TEXT("로그 3: Found 함수 호출 시작. (키 입력 성공)"));
	if (!inputValue.Get<bool>())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	FVector startPos = CamComp->GetComponentLocation();
	// 시야 350cm 내에서 검색
	FVector endPos = startPos + CamComp->GetForwardVector() * 350;

	FHitResult hitInfo;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//Line Trace 수행(_> 교수님 코드 참고)
	bool bHit = World->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, Params);
	if (bHit)
	{
		//로그 4A: Line Trace 성공_
		AActor* HitActor = hitInfo.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("로그 4: Line Trace 성공. 충돌 대상 액터: %s"), HitActor ? *HitActor->GetName() : TEXT("None"));

		if (!HitActor || !HitActor->IsValidLowLevelFast())
		{
			return;
		}

		AEvidenceActor* Evidence = Cast<AEvidenceActor>(HitActor);

		if (Evidence) {
			//로그 5: EvidenceActor 캐스트 성공
			UE_LOG(LogTemp, Warning, TEXT("로그 5: EvidenceActor 캐스트 성공. 증거 이름: %s"), *Evidence->GetName());

			if (!Evidence->bIsFound)
			{
				//로그 6: 증거 획득 시도 (아직 획득하지 않음)
				UE_LOG(LogTemp, Warning, TEXT("로그 6: 증거 획득 로직 AddEvidenceToInventory 호출."));
				AddEvidenceToInventory(Evidence);
			}
			else
			{
				// 로그 7: 이미 획득한 증거
				UE_LOG(LogTemp, Display, TEXT("로그 7: 이미 획득한 증거입니다."));
			}
		}
		else //NPC 루트 -> Line Trace에서 마지막 그게 부모 클래스가 HitNPC이면 실행
		{
			ABaseNPC* HitNPC = Cast<ABaseNPC>(HitActor);

			if (HitNPC)
			{
				// 2. NPC가 맞다면, InitiateDialogue 함수를 호출합니다.
				UE_LOG(LogTemp, Warning, TEXT("로그 8.1: NPC와 충돌. 대화 시작 InitiateDialogue 호출."));
				HitNPC->InitiateDialogue();
			}
		}//Line Trace끝
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("충돌 대상은 EvidenceActor나 BaseNPC ->X."));
	}
}

void AMyPlayer::AddEvidenceToInventory(AEvidenceActor* Evidence) //인벤토리 테이블 List 검사
{
	if (!Evidence) return;

	//데이터 테이블 유효성 검사
	if (!EvidenceDataTable || !EvidenceDataTable->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("로그 9: CRITICAL ERROR! EvidenceDataTable이 BP_MyPlayer에 할당되지 않았거나 유효하지 않습니다."));
		return;
	}

	const FName EvidenceRowName = Evidence->EvidenceDataKey;
	FEvidenceData* EvidenceData = EvidenceDataTable->FindRow<FEvidenceData>(EvidenceRowName, TEXT("Find Evidence Data"));

	// 데이터 테이블 키 확인
	UE_LOG(LogTemp, Warning, TEXT("로그 10: 찾으려는 데이터 테이블 키: %s"), *EvidenceRowName.ToString()); //<-내가 클릭한 증거와 찾으려는 테이블 키 비교


	if (EvidenceData)
	{
		Evidence->bIsFound = true;

		// 증거 획득 성공 및 BP 이벤트 호출
		UE_LOG(LogTemp, Warning, TEXT("로그 11: 증거 획득 성공! OnEvidenceFound (블루프린트 이벤트) 호출!"));

		// OnEvidenceFound (블루프린트 이벤트) 호출
		OnEvidenceFound(*EvidenceData);//여기서 블루프린트로 열어서 List저장~~

		// 획득 후 월드 액터 처리
		Evidence->SetActorEnableCollision(false);
		if (Evidence->EvidenceMesh)
		{
			Evidence->EvidenceMesh->SetCollisionResponseToChannel(
				ECollisionChannel::ECC_Visibility,
				ECollisionResponse::ECR_Ignore
			);
		}
	}
	else
	{
		//데이터 테이블에서 행 찾기 실패
		UE_LOG(LogTemp, Error, TEXT("로그 12: 데이터 테이블에서 키 '%s'를 찾을 수 없습니다. 키 이름과 데이터 테이블을 다시 확인하세요."), *Evidence->EvidenceDataKey.ToString());
	}
}

void AMyPlayer::AdvanceDayCheat(const struct FInputActionValue& inputValue)
{
	if (!inputValue.Get<bool>())
	{
		return;
	}
	if (UMy_GameInstance* GameInstance = Cast<UMy_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// 2. Game Instance의 날짜 진행 및 TP 함수를 호출
		GameInstance->AdvanceToNextDayAndTeleport();
		UE_LOG(LogTemp, Warning, TEXT("치트 성공: Advance Day Cheating 활성화! CurrentDay: %d"), GameInstance->CurrentDay);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("치트 실패: Game Instance Casting 실패. 프로젝트 설정을 확인하세요."));
	}
}

