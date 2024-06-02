#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class VR_MOTION_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 변환된 좌표를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetReceivedData(const TArray<FVector>& Data);

	// Getter 함수
	UFUNCTION(BlueprintCallable, Category = "Data")
	const TArray<FVector>& GetReceivedData() const;

	// 특정 인덱스의 데이터를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetReceivedDataAtIndex(int32 Index, const FVector& Data);

	// 특정 인덱스의 데이터를 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Data")
	FVector GetReceivedDataAtIndex(int32 Index) const;

private:
	// 수신된 데이터를 저장할 변수, 블루프린트에서 볼 수 있고 수정 가능하도록 설정
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true", ToolTip = "Array of received data points"))
	TArray<FVector> ReceivedData;
};
