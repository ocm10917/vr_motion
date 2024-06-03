#include "UDPActor.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "SCharacter.h" // ASCharacter 헤더 포함
#include "EngineUtils.h"  // TActorIterator 사용을 위해 필요

AUDPActor::AUDPActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUDPActor::BeginPlay()
{
	Super::BeginPlay();
	StartUDPReceiver(TEXT("UDPSocket"), TEXT("127.0.0.1"), 9876);  // UDP 리시버 시작
}

void AUDPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UDP 수신 후 ASCharacter로 데이터 전송
	if (ReceivedData.Num() > 0)
	{
		SendDataToSCharacter(ReceivedData);
		ReceivedData.Empty();  // 데이터 처리 후 비워줌
	}
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
	TArray<float> RawData;
	RawData.Empty();

	// float 배열의 길이를 구합니다.
	int32 NumFloats = ArrayReaderPtr->Num() / sizeof(float);
	
	// 바이트 데이터를 float 배열로 변환합니다.
	for (int32 i = 0; i < NumFloats; ++i)
	{
		float Value;
		*ArrayReaderPtr << Value;
		RawData.Add(Value);
	}

	// 기존 데이터를 비웁니다.
	ReceivedData.Empty();

	// 좌표 변환을 수행합니다.
	for (int32 i = 0; i < RawData.Num(); i += 3)
	{
		float MediapipeX = RawData[i];
		float MediapipeY = RawData[i + 1];
		float MediapipeZ = RawData[i + 2];

		FVector ConvertedCoord = ConvertMediapipeToUnreal(MediapipeX, MediapipeY, MediapipeZ);
		ReceivedData.Add(ConvertedCoord);
	}
}

FVector AUDPActor::ConvertMediapipeToUnreal(float X, float Y, float Z)
{
	FVector ScaleVector(100.f, 100.f, 100.f);
	return FVector(Y * ScaleVector.X, Z * ScaleVector.Y, -X * ScaleVector.Z);
}

void AUDPActor::SendDataToSCharacter(const TArray<FVector>& Data)
{
	// 현재 맵에서 ASCharacter를 찾습니다.
	for (TActorIterator<ASCharacter> It(GetWorld()); It; ++It)
	{
		ASCharacter* Character = *It;
		if (Character)
		{
			Character->SetReceivedData(Data);
		}
	}
}
