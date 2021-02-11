// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class UNREALCUTTLE_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent* RightMotionController;

	/* Meshes that represent the hands */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetInteractionComponent* wInteraction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URemoteProcedureComponent* RemoteProcedureCall;

	UFUNCTION(BlueprintCallable, Category = "Laser")
	void ShowLaser();

	UFUNCTION(BlueprintCallable, Category = "Laser")
	void HideLaser();
};
