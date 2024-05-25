#include "UDPActor.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

AUDPActor::AUDPActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUDPActor::BeginPlay()
{
	Super::BeginPlay();
	StartUDPReceiver(TEXT("UDPSocket"), TEXT("0.0.0.0"), 9876);
}

void AUDPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUDPActor::StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	FIPv4Endpoint Endpoint(FIPv4Address::Any, ThePort);

	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(2 * 1024 * 1024);

	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(2 * 1024 * 1024, NewSize);

	UDPReceiver = new FUdpSocketReceiver(ListenSocket, FTimespan::FromMilliseconds(100), TEXT("UDP RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &AUDPActor::Recv);
	UDPReceiver->Start();
}

void AUDPActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	delete UDPReceiver;
	UDPReceiver = nullptr;

	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

void AUDPActor::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	TArray<float> ReceivedData;

	// float 배열의 길이를 구합니다.
	int32 NumFloats = ArrayReaderPtr->Num() / sizeof(float);

	// 바이트 데이터를 float 배열로 변환합니다.
	for (int32 i = 0; i < NumFloats; ++i)
	{
		float Value;
		*ArrayReaderPtr << Value;
		ReceivedData.Add(Value);
	}

	// 수신한 float 배열을 출력합니다.
	for (float Value : ReceivedData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Received Float value: %f"), Value);
	}
}


