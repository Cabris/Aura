// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

void AAuraEnemy::HightlightActor()
{
	HightlightMesh(GetMesh());
	HightlightMesh(Weapon);
	bHightlight = true;
	OnHightlightChanged(bHightlight);
}

void AAuraEnemy::UnHightlightActor()
{
	UnHightlightMesh(GetMesh());
	UnHightlightMesh(Weapon);
	bHightlight = false;
	OnHightlightChanged(bHightlight);
}

AAuraEnemy::AAuraEnemy()
{
	auto* mesh = GetMesh();
	check(mesh);
	mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HightlightMesh(UPrimitiveComponent* mesh)
{
	check(mesh);
	mesh->SetRenderCustomDepth(true);
	mesh->SetCustomDepthStencilValue(iStencilValue);
}

void AAuraEnemy::UnHightlightMesh(UPrimitiveComponent* mesh)
{
	check(mesh);
	mesh->SetRenderCustomDepth(false);
	mesh->SetCustomDepthStencilValue(0);
}
