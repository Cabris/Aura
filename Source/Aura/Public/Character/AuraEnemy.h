// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacterBase.h"
#include "Interfactions/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	//IEnemyInterface
	virtual void HightlightActor();
	virtual void UnHightlightActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly, Category="Hightlight")
	bool bHightlight;
	UPROPERTY(EditAnywhere, Category="Hightlight")
	int32 iStencilValue = CUSTOM_DEPTH_RED;
	UFUNCTION(BlueprintImplementableEvent, Category="Hightlight")
	void OnHightlightChanged(const bool bIsHightlighted);

private:
	void HightlightMesh(UPrimitiveComponent* Mesh);
	void UnHightlightMesh(UPrimitiveComponent* Mesh);
};
