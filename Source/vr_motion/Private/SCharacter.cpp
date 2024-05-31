#include "SCharacter.h"
#include "Components/SkeletalMeshComponent.h" // SkeletalMeshComponent 헤더 추가

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (float Value : ReceivedData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Received float value in Tick: %f"), Value);
	}
}

// Called to bind functionality to input
// void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
// }

void ASCharacter::SetReceivedData(const TArray<float>& Data)
{
	ReceivedData = Data;
}

