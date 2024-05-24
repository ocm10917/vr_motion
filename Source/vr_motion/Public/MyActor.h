#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "MyActor.generated.h"

UCLASS()
class AMyActor : public AActor
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FSocket* ListenSocket;
	FUdpSocketReceiver* UDPReceiver;

	void StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);
	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);
};

