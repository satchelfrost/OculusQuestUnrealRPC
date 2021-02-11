// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RemoteProcedureComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCUTTLE_API URemoteProcedureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URemoteProcedureComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Socket for the TCP connection
	FSocket* ClientSocket;

	// Address of the TCP server, possibly shared by multiple threads
	TSharedPtr<FInternetAddr> RemoteAddress;

	// True means we are listening for incoming data
	FThreadSafeBool bShouldReceiveData;

	// A promise to keep listening for incoming data
	TFuture<void> ClientConnectionFinishedFuture;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Used to cancel our connection to the server
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
	// Open a TCP connection to the server with default IP Address and Port
	void ConnectToServer(const FString& IP = TEXT("127.0.0.1"), const int32 Port = 59898);

	// Cancels connection to the server
	void CloseSocket();

	// Sends string with newline character to server, and returns boolean based on send success 
	UFUNCTION(BlueprintCallable, Category = "Send Functions")
	bool SendString(FString str);

	// Convert a byte array to human readable string
	FString fBytesToString(const TArray<uint8>& InArray);

	// For packaging our string before sending to the socket
	TArray<uint8> fStringToBytes(FString InString);
};
