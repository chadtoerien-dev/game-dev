#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TheVeilCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class THEVEIL_API ATheVeilCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATheVeilCharacter();

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void Move(const FVector2D& MovementInput);

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void Look(const FVector2D& LookInput);

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void StopSprint();

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void ToggleCrouch();

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void StartJump();

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void StopJump();

    UFUNCTION(BlueprintCallable, Category="The Veil|Movement")
    void ResetTransientMovementState();

    UFUNCTION(BlueprintPure, Category="The Veil|Movement")
    bool IsSprinting() const { return bWantsToSprint; }

    UFUNCTION(BlueprintPure, Category="The Veil|Camera")
    USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    UFUNCTION(BlueprintPure, Category="The Veil|Camera")
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    virtual void Restart() override;
    virtual void PawnClientRestart() override;

protected:
    virtual void PostInitializeComponents() override;
    virtual void OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", Units="cm/s"))
    float WalkSpeed = 500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", Units="cm/s"))
    float SprintSpeed = 720.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", Units="cm/s"))
    float CrouchedSpeed = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", Units="cm/s^2"))
    float MovementAcceleration = 2200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", Units="cm/s^2"))
    float WalkingBrakingDeceleration = 2000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", Units="deg/s"))
    float YawRotationRate = 600.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0"))
    float GroundFriction = 8.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Movement|Tuning", meta=(ClampMin="0.0", ClampMax="1.0"))
    float AirControl = 0.35f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Camera|Tuning", meta=(ClampMin="0.0", Units="cm"))
    float CameraBoomLength = 340.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Camera|Tuning", meta=(ClampMin="0.0", Units="cm"))
    float CameraCollisionProbeSize = 12.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Camera|Tuning", meta=(ClampMin="0.0"))
    float CameraPositionLagSpeed = 18.0f;

private:
    void ApplyTuning();
    void UpdateMovementSpeed();

    UPROPERTY(Transient)
    bool bWantsToSprint = false;
};
