#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "BattleExperienceManagerComponent.generated.h"


class UBattleExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBattleExperienceLoaded, const UBattleExperienceDefinition* );

enum class EBattleExperienceLoadState
{
	Unloaded,
	Loading,
	//LoadingGameFeature,
	//ExecutingActions,
	Loaded,
	Deactivating,
};

/*
 * GameState를 Owner로 가지며, Experience의 상태 정보를 가지고 관리하는 Component
 *  - Experience Loading 에 관여함.
 */

UCLASS()
class UBattleExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UBattleExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 로딩 완료, Experience가 존재.
	bool IsExperienceLoaded() const {return (LoadState == EBattleExperienceLoadState::Loaded) &&(CurrentExperience != nullptr);}

	// OnExperienceLoaded 델리게이트에 바인딩하거나 Experience가 로딩됐으면 바로 델리게이트를 실행시킴(Notify)
	void CallOrRegister_OnExperienceLoaded(FOnBattleExperienceLoaded::FDelegate&& Delegate);
	
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
	const UBattleExperienceDefinition* GetCurrentExperienceChecked() const;
	
private:
	
	UPROPERTY()
	TObjectPtr<const UBattleExperienceDefinition> CurrentExperience;

	// 로딩 상태 모니터링
	EBattleExperienceLoadState LoadState = EBattleExperienceLoadState::Unloaded;

	// Experience 로딩 완료 후 델리게이트
	FOnBattleExperienceLoaded OnExperienceLoaded;

	
	
};
