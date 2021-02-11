// Fill out your copyright notice in the Description page of Project Settings.

#include "RemoteProcedureComponent.h"
#include "Sockets.h"
#include "HAL/ThreadSafeBool.h"
#include "Async/Async.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "Misc/FileHelper.h"
#include "IPAddress.h"

// Sets default values for this component's properties
URemoteProcedureComponent::URemoteProcedureComponent()
	: ClientSocket(nullptr), bShouldReceiveData(true)
{
}

// Called when the game starts
void URemoteProcedureComponent::BeginPlay()
{
	Super::BeginPlay();

	// Connect to server using default IP and Port
	ConnectToServer("192.168.0.11");
}

// Called every frame
void URemoteProcedureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URemoteProcedureComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CloseSocket();
}

void URemoteProcedureComponent::ConnectToServer(const FString& IP, const int32 Port)
{
	// Try setting up the remote address
	RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	RemoteAddress->SetIp(*IP, bIsValid);
	RemoteAddress->SetPort(Port);
	if (!bIsValid) {
		UE_LOG(LogTemp, Warning, TEXT("TCP address is invalid %s:%d"), *IP, Port)
		return;
	}

	// Setup the client socket
	FString ClientSocketName(TEXT("tcp-client"));
	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, ClientSocketName, false);
	int32 MaxBufferSize = 2 * 1024 * 1024; // 2 MiB
	ClientSocket->SetSendBufferSize(MaxBufferSize, MaxBufferSize);
	ClientSocket->SetReceiveBufferSize(MaxBufferSize, MaxBufferSize);

	// Connect to the remote address
	if (ClientSocket->Connect(*RemoteAddress)) {
		UE_LOG(LogTemp, Warning, TEXT("Successfully connected to %s:%d"), *IP, Port)
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Could not connect to %s:%d"), *IP, Port)
		return;
	}

	ClientConnectionFinishedFuture = Async(EAsyncExecution::Thread, [&]()
		{
			uint32 BufferSize = 0;
			TArray<uint8> ReceiveBuffer;
			FString ResultString;

			// Where we may receive incoming data
			while (bShouldReceiveData) {
				if (ClientSocket->HasPendingData(BufferSize)) {
					ReceiveBuffer.SetNumUninitialized(BufferSize);
					int32 Read = 0;
					ClientSocket->Recv(ReceiveBuffer.GetData(), ReceiveBuffer.Num(), Read);

					// Send the "fBytesToString(ReceiveBuffer)" here. 
					UE_LOG(LogTemp, Warning, TEXT("%s"), *fBytesToString(ReceiveBuffer))
				}

				// Skip a tick
				ClientSocket->Wait(ESocketWaitConditions::WaitForReadOrWrite, FTimespan(1));
			}
		}
	);
}

void URemoteProcedureComponent::CloseSocket()
{
	if (ClientSocket) {
		bShouldReceiveData = false;

		// finish off the future
		if (ClientConnectionFinishedFuture.IsValid())
			ClientConnectionFinishedFuture.Get();

		ClientSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
		ClientSocket = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Socket Closed"))
	}
}

bool URemoteProcedureComponent::SendString(FString str)
{
	bool bSendSuccess = false;

	// First check if the socket is connected
	if (ClientSocket && ClientSocket->GetConnectionState() == SCS_Connected) {
		int32 BytesSent = 0;
		TArray<uint8> Bytes = fStringToBytes(str);
		Bytes.Add('\n'); // Add a newline character to the string
		bSendSuccess = ClientSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent);
	}

	return bSendSuccess;
}

FString URemoteProcedureComponent::fBytesToString(const TArray<uint8>& InArray)
{
	FString ResultString;
	FFileHelper::BufferToString(ResultString, InArray.GetData(), InArray.Num());
	return ResultString;
}

TArray<uint8> URemoteProcedureComponent::fStringToBytes(FString InString)
{
	TArray<uint8> ResultBytes;
	ResultBytes.Append((uint8*)TCHAR_TO_UTF8(*InString), InString.Len());
	return ResultBytes;
}
