// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ColourFloor/ColourFloor.h"
#include "InputActionValue.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ColourBomb/ColourBomb.h"
#include "ColourCube/ColourCube.h"
#include "NiagaraFunctionLibrary.h"
#include "Assignment1BCharacter.generated.h"


UCLASS(config=Game)
class AAssignment1BCharacter : public ACharacter, public IColourInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Activate Bomb Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ActivateBombAction;

	/** Spawn Cube Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpawnCubeAction;

	/** Respawn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RespawnAction;

public:
	AAssignment1BCharacter();
	
	void StartController();

	UPROPERTY(EditAnywhere)USkeletalMeshComponent* MainMesh;
	
	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;
	
	UPROPERTY()UMaterialInstanceDynamic* MaterialInstance;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
private:
	// Trace basic module
	bool Trace(UWorld* World,TArray<AActor*>& ActorsToIgnore,
				const FVector& Start,const FVector& End,
				FHitResult& HitOut,ECollisionChannel CollisionChannel,bool ReturnPhysMat);
	void CallMyTrace(int ActionNumber);
	
	// Tile check module
	void ProcessTileHit(FHitResult& HitOut);
	UPROPERTY()AColourFloor* TileUnderFoot; // Record which floor tile the player is on
	
	// Activate bomb module
	UPROPERTY(EditAnywhere)UNiagaraSystem* NS_ActivateBomb;
	void ActivateBomb();

	// Activate bomb cool down part
	FTimerHandle ActivateBombTimerHandle;
	UPROPERTY(EditAnywhere)float ActivateBombCoolDown = 4.0f; // Default cool down is 4 seconds
	void ActivateBombCoolDownFinish();
	bool bIsBombCoolDownFinished = true;
	void ProcessBombCoolDown();
	
	// Spawn cube module
	UPROPERTY(EditAnywhere)int SpawnDistance = 1200;
	void ChooseCubeSpawnLocation();
	void ProcessSpawnCubeHit(FHitResult& HitOut);
	
	// Spawn cube cool down part (actually the floor manager will spawn cube to record its location more conveniently)
	bool bIsCubeCoolDownFinished = true;
	FTimerHandle SpawnCubeTimerHandle;
	UPROPERTY(EditAnywhere)float SpawnCubeCoolDown = 1.5f; // Default cool down is 1.5 seconds
	void SpawnCubeCoolDownFinish();

	// Generate cube colour module
	FLinearColor GenerateRandomCubeColour();

	// Change the max walk speed
	void UpdateWalkSpeed();

public:
	// Spawn bomb module
	UPROPERTY(EditAnywhere)TSubclassOf<class AColourBomb> BombClass;
	UPROPERTY()AColourBomb* SpawnedBomb;
	void SpawnBomb(FLinearColor TeamColour);

	// Spawn cube public part for floor manager to read
	bool bIsSpawningCube = false;
	FLinearColor SpawnCubeColour; // The colour of the current cube spawned will be
	FLinearColor NextCubeColour; // The colour of the next cube to display through the crosshair
	FVector CubeSpawnLocation;
	UPROPERTY()AColourFloor* CubeSpawnTile;

	// Respawn module
	void Respawn();
	void ProcessRespawn(float DeltaSeconds);
	FVector RespawnLocation;
	bool bIsRespawning = false;
	bool bIsInMatchEnd = false; // Check if the match just ends
	bool bIsInMatch = false; // Check if in the match stage
	bool bIsInPrepare = false; // Check if in the prepare stage
	float MatchTimeRemain; // Store the time of this match remained
	float PrepareTimeRemain; // Store the time of this match remained
	int WhichTeamWin = 0;

	// Cube power up ability unlock module
	void CubeAbilityUnlock();
	void CubeAbilityFinish();
	FTimerHandle CubeAbilityTimerHandle;
	UPROPERTY(EditAnywhere)float CubeAbilityTimeRemained = 15.0f;
	bool bIsSpawnSameCubesUnlocked = false;

	// Bomb power up ability
	void BombAbilityUnlock();
	bool bIsSpawnEnhancedBombUnlocked = false;
	UPROPERTY(EditAnywhere)TSubclassOf<class AEnhancedBomb> EnhancedBombClass;
};

