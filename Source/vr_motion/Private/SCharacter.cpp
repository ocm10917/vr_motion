#include "SCharacter.h"

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::SetReceivedData(const TArray<FVector>& Data)
{
	ReceivedData = Data;
}

const TArray<FVector>& ASCharacter::GetReceivedData() const
{
	return ReceivedData;
}

void ASCharacter::SetReceivedDataAtIndex(int32 Index, const FVector& Data)
{
	if (ReceivedData.IsValidIndex(Index))
	{
		ReceivedData[Index] = Data;
	}
}

FVector ASCharacter::GetReceivedDataAtIndex(int32 Index) const
{
	if (ReceivedData.IsValidIndex(Index))
	{
		return ReceivedData[Index];
	}
	
	return FVector::ZeroVector; // 유효하지 않은 인덱스일 경우 기본값 반환
}