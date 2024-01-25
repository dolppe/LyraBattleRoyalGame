#pragma once
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"

#include "BattleHeroComponent.generated.h"

//  카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 담당.
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UBattleHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UBattleHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;

	// PawnComponent Interface
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// IGameFrameworkInitStateInterface Interface
	virtual FName GetFeatureName() const final {return NAME_ActorFeatureName;}
	virtual void CheckDefaultInitialization() final;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;

};