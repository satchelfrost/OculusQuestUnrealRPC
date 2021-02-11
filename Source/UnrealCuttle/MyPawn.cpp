// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "RemoteProcedureComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Scene);

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	LeftMotionController->SetupAttachment(Scene);
	LeftMotionController->SetTrackingSource(EControllerHand::Left);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));
	RightMotionController->SetupAttachment(Scene);
	RightMotionController->SetTrackingSource(EControllerHand::Right);

	LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand Mesh"));
	LeftHand->SetupAttachment(LeftMotionController);

	RightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand Mesh"));
	RightHand->SetupAttachment(RightMotionController);

	/* Right hand Interaction with a widget */
	wInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction Component"));
	wInteraction->SetupAttachment(RightHand);
;	wInteraction->InteractionDistance = 750.f;
	wInteraction->bShowDebug = false;

	RemoteProcedureCall = CreateDefaultSubobject<URemoteProcedureComponent>(TEXT("RPC Component"));
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyPawn::ShowLaser()
{
	wInteraction->bShowDebug = true;
}

void AMyPawn::HideLaser()
{
	wInteraction->bShowDebug = false;
}
