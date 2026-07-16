#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Core/TVGameplayTypes.h"
#include "TVContestantStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVContestantStateChanged, FName, ChangeReason);

UCLASS(ClassGroup=(TheVeil), meta=(BlueprintSpawnableComponent))
class THEVEIL_API UTVContestantStateComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UTVContestantStateComponent();
    UPROPERTY(BlueprintAssignable, Category="The Veil|Contestant") FTVContestantStateChanged OnContestantStateChanged;
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") FName GetContestantId() const { return ContestantId; }
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") float GetTrustToPlayer() const { return TrustToPlayer; }
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") float GetFearOfPlayer() const { return FearOfPlayer; }
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") FGameplayTagContainer GetKnowledgeTags() const { return KnowledgeTags; }
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") FGameplayTagContainer GetAccessGrantTags() const { return AccessGrantTags; }
    UFUNCTION(BlueprintCallable, Category="The Veil|Contestant") void SetTrustToPlayer(float NewValue, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Contestant") void AddTrustToPlayer(float Delta, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Contestant") void SetFearOfPlayer(float NewValue, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Contestant") void SetKnowledgeTag(FGameplayTag Tag, bool bEnabled, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Contestant") void SetAccessGrant(FGameplayTag Tag, bool bEnabled, FName Reason);
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") bool HasKnowledgeTag(FGameplayTag Tag) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") bool HasAccessGrant(FGameplayTag Tag) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Contestant") FTVContestantStateSnapshot CreateSnapshot() const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Contestant") bool RestoreSnapshot(const FTVContestantStateSnapshot& Snapshot, bool bBroadcastChange);
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Contestant") FName ContestantId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Contestant", meta=(ClampMin="0.0", ClampMax="100.0")) float TrustToPlayer = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Contestant", meta=(ClampMin="0.0", ClampMax="100.0")) float FearOfPlayer = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Contestant") FGameplayTagContainer KnowledgeTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Contestant") FGameplayTagContainer AccessGrantTags;
private:
    void BroadcastChange(FName Reason);
};
