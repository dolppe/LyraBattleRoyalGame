// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraBattleRoyalGame.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "LyraBattleRoyalGame/System/BattleAssetManager.h"

class FLyraBattleRoyalGameModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};

void FLyraBattleRoyalGameModule::StartupModule()
{
}

void FLyraBattleRoyalGameModule::ShutdownModule()
{
}

IMPLEMENT_PRIMARY_GAME_MODULE( FLyraBattleRoyalGameModule, LyraBattleRoyalGame, "LyraBattleRoyalGame" );


