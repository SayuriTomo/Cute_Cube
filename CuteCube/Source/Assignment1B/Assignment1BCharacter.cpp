// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment1BCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Image.h"


//////////////////////////////////////////////////////////////////////////
// AAssignment1BCharacter

AAssignment1BCharacter::AAssignment1BCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	MainMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Main Mesh"));
	MainMesh -> SetupAttachment(RootComponent);
	
}

void AAssignment1BCharacter::StartController()
{
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAssignment1BCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		MainMesh->SetMaterial(0,MaterialInstance);
	}
	NextCubeColour = GenerateRandomCubeColour();
}

void AAssignment1BCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	ProcessRespawn(DeltaSeconds);
	ProcessBombCoolDown();
	
	// Change and record the tile under foot
	CallMyTrace(1);
	
	UpdateWalkSpeed();
}

bool AAssignment1BCharacter::Trace(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start,const FVector& End, FHitResult& HitOut, ECollisionChannel CollisionChannel, bool ReturnPhysMat)
{
	// The World parameter refers to our game world (map/level) If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	
	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel (
		HitOut, //result
		Start, //start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

void AAssignment1BCharacter::CallMyTrace(int ActionNumber)
{
	// Initialize the vector of the starting and ending of the ray
	FVector Start;
	FVector End;

	switch (ActionNumber)
	{
		// Check the tile under foot
		case 1:
			{
				Start = this->GetActorLocation();
				End = Start - FVector(0,0,150);
				break;
			}
		case 2:
			{
				Start = FollowCamera->GetComponentLocation();
				End = Start + FollowCamera->GetForwardVector() * SpawnDistance;
			}
		default:{}
	}
	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);
	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;
	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		// Process our HitData
		if (HitData.GetActor())
		{
			switch (ActionNumber)
			{
				// Record and change the tile under foot
				case 1:
					{
						ProcessTileHit(HitData);
						break;
					}
				case 2:
					{
						ProcessSpawnCubeHit(HitData);
						break;
					}
				default:{}
			}
		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	}
	else
	{
		// We did not hit an Actor
	}
}

void AAssignment1BCharacter::SpawnBomb(FLinearColor TeamColour)
{
	this -> ColourDisplayed = TeamColour;
	if(MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("ColourDisplayed", ColourDisplayed);
	}
	if(BombClass)
	{
		SpawnedBomb = GetWorld()->SpawnActor<AColourBomb>(BombClass, this->GetActorLocation(), this->GetActorRotation());
		SpawnedBomb -> ColourDisplayed = TeamColour;
	}
}

void AAssignment1BCharacter::ActivateBomb()
{
	if(NS_ActivateBomb&&SpawnedBomb&&TileUnderFoot&&bIsBombCoolDownFinished&&!bIsInPrepare)
	{
		if(!TileUnderFoot->bIsBombPlacedOn
			&&!TileUnderFoot->bIsCubePlacedOn)
		{
			// Generate the spawn location and rotation
			const FVector SpawnLocation = TileUnderFoot->GetActorLocation()+FVector(0,0,50);
			const FRotator SpawnRotation = TileUnderFoot->GetActorRotation();

			// Spawn the niagara effect and initialize its colour
			UNiagaraComponent* BombComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_ActivateBomb, SpawnLocation, SpawnRotation);
			BombComponent->SetNiagaraVariableLinearColor(FString("ColourDisplayed"),ColourDisplayed);

			// Activate the bomb
			SpawnedBomb->BeActivated(TileUnderFoot);
			
			// Start cool down
			bIsBombCoolDownFinished = false;
			GetWorld()->GetTimerManager().SetTimer(ActivateBombTimerHandle, this, &AAssignment1BCharacter::ActivateBombCoolDownFinish, ActivateBombCoolDown, true);
		}
		
	}
}

void AAssignment1BCharacter::ActivateBombCoolDownFinish()
{
	// The player can activate the bomb again
	bIsBombCoolDownFinished = true;
	
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(ActivateBombTimerHandle);
}

void AAssignment1BCharacter::ProcessBombCoolDown()
{
	if(!bIsBombCoolDownFinished)
	{
		if(MaterialInstance)
		{
			MaterialInstance->SetVectorParameterValue("ColourDisplayed", FLinearColor::Gray);
		}
	}
	else
	{
		if(MaterialInstance)
		{
			MaterialInstance->SetVectorParameterValue("ColourDisplayed", ColourDisplayed);
		}
	}
}

FLinearColor AAssignment1BCharacter::GenerateRandomCubeColour()
{
	// Generate the colour of the next cube spawned
	switch (FMath::RandRange(0,2))
	{
		case 0:
			{
				return FLinearColor::Red;
			}
		case 1:
			{
				return FLinearColor::Blue;
			}
		case 2:
			{
				return FLinearColor::Gray;
			}
		default:{}
	}
	return FLinearColor::Gray;
}

void AAssignment1BCharacter::UpdateWalkSpeed()
{
	if(TileUnderFoot)
	{
		if(TileUnderFoot->ColourDisplayed == ColourDisplayed)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
		}
		else
		{
			if(TileUnderFoot->ColourDisplayed == FLinearColor::Gray)
			{
				GetCharacterMovement()->MaxWalkSpeed = 350.f;
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = 200.f;
			}
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}

void AAssignment1BCharacter::ChooseCubeSpawnLocation()
{
	if(bIsCubeCoolDownFinished&&!bIsInPrepare)
	{
		CallMyTrace(2);
	}
}

void AAssignment1BCharacter::ProcessSpawnCubeHit(FHitResult& HitOut)
{
	// Check if the hit Location can place cubes
	// Change the location of cube spawned
	if(bool bIsLocationCorrect = false;Cast<AColourFloor>(HitOut.GetActor())||Cast<AColourCube>(HitOut.GetActor()))
	{
		if(Cast<AColourFloor>(HitOut.GetActor())
			&&!Cast<AColourFloor>(HitOut.GetActor())->bIsBombPlacedOn
			&&!Cast<AColourFloor>(HitOut.GetActor())->bIsCubePlacedOn)
		{
			CubeSpawnLocation = HitOut.GetActor()->GetActorLocation() + HitOut.Normal*100;
			CubeSpawnTile = Cast<AColourFloor>(HitOut.GetActor());
			bIsLocationCorrect = true;
		}
		else if(Cast<AColourCube>(HitOut.GetActor()))
		{
			// Check whether the normal is toward the right direction
			if(HitOut.Normal.Equals(FVector_NetQuantizeNormal(0,0,1.f)))
			{
				CubeSpawnLocation = HitOut.GetActor()->GetActorLocation()
										+ FVector(round(HitOut.Normal.X)*200,
										          round(HitOut.Normal.Y)*200,
										          round(HitOut.Normal.Z)*200);
				bIsLocationCorrect = true;
			}
		}

		if(bIsLocationCorrect)
		{
			SpawnCubeColour = NextCubeColour;
			bIsSpawningCube = true;
			
			// Generate the colour of the next cube spawned
			NextCubeColour = GenerateRandomCubeColour();
			
			// Start cool down
			bIsCubeCoolDownFinished = false;
			GetWorld()->GetTimerManager().SetTimer(SpawnCubeTimerHandle, this, &AAssignment1BCharacter::SpawnCubeCoolDownFinish, SpawnCubeCoolDown, true);
		}
	}
}

void AAssignment1BCharacter::SpawnCubeCoolDownFinish()
{
	// The player can spawn the cube again
	bIsCubeCoolDownFinished = true;
	
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(SpawnCubeTimerHandle);
}

void AAssignment1BCharacter::Respawn()
{
	if(!bIsInPrepare)
	{
		bIsRespawning = true;
		PrepareTimeRemain = 5.0f;
	}
}

void AAssignment1BCharacter::ProcessRespawn(float DeltaSeconds)
{
	if(bIsRespawning)
	{
		PrepareTimeRemain -= DeltaSeconds;
		if(PrepareTimeRemain <= 0)
		{
			bIsInPrepare = false;
			bIsRespawning = false;
		}
	}
}

void AAssignment1BCharacter::ProcessTileHit(FHitResult& HitOut)
{
	// Change the tile under foot
	if(Cast<AColourFloor>(HitOut.GetActor()))
	{
		TileUnderFoot = Cast<AColourFloor>(HitOut.GetActor());
	}
	else
	{
		TileUnderFoot = nullptr;
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AAssignment1BCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAssignment1BCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAssignment1BCharacter::Look);

		//Activate Bomb
		EnhancedInputComponent->BindAction(ActivateBombAction,ETriggerEvent::Triggered,this,&AAssignment1BCharacter::ActivateBomb);

		//Spawn Cube
		EnhancedInputComponent->BindAction(SpawnCubeAction,ETriggerEvent::Triggered,this,&AAssignment1BCharacter::ChooseCubeSpawnLocation);

		//Respawn
		EnhancedInputComponent->BindAction(RespawnAction,ETriggerEvent::Triggered,this,&AAssignment1BCharacter::Respawn);
	}

}

void AAssignment1BCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if(!bIsInPrepare)
		{
			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AAssignment1BCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




