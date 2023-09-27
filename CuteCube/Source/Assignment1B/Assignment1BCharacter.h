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

public:
	AAssignment1BCharacter();
	
	void StartController();
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
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
	bool bIsBombCoolDownFinished = true;
	FTimerHandle ActivateBombTimerHandle;
	UPROPERTY(EditAnywhere)float ActivateBombCoolDown = 5.0f; // Default cool down is 5 seconds
	void ActivateBombCoolDownFinish();

	// Generate cube colour module
	void GenerateRandomCubeColour();

	// Spawn cube module
	UPROPERTY(EditAnywhere)int SpawnDistance = 800;
	void ChooseCubeSpawnLocation();
	void ProcessSpawnCubeHit(FHitResult& HitOut);
	
	// Spawn cube cool down part (actually the floor manager will spawn cube to record its location more conveniently)
	bool bIsCubeCoolDownFinished = true;
	FTimerHandle SpawnCubeTimerHandle;
	UPROPERTY(EditAnywhere)float SpawnCubeCoolDown = 2.0f; // Default cool down is 2 seconds
	void SpawnCubeCoolDownFinish();

public:
	// Spawn bomb module
	UPROPERTY(EditAnywhere)TSubclassOf<class AColourBomb> BombClass;
	UPROPERTY()AColourBomb* SpawnedBomb;
	void SpawnBomb(FLinearColor TeamColour);

	// Spawn cube public part for floor manager to read
	bool bIsSpawningCube = false;
	FLinearColor SpawnCubeColour;
	FVector CubeSpawnLocation;
	
};
