#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "UDPActor.generated.h"

UCLASS()
class AUDPActor : public AActor
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AUDPActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FSocket* ListenSocket;
	FUdpSocketReceiver* UDPReceiver;
	TArray<FVector> ReceivedData;

	void StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);
	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);
	void SendDataToSCharacter(const TArray<FVector>& Data);

	FVector ConvertMediapipeToUnreal(float X, float Y, float Z);
};
