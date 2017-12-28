// Fill out your copyright notice in the Description page of Project Settings.

#include "AHCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AAHCharacter::AAHCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void AAHCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAHCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AAHCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AAHCharacter::BeginCrouch()
{
	Crouch();
}

void AAHCharacter::EndCrouch()
{
	UnCrouch();
}

// Called every frame
void AAHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAHCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAHCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AAHCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AAHCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AAHCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AAHCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAHCharacter::Jump);
}

