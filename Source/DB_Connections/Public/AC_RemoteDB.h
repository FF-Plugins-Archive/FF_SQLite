// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Connection Functions.
#include "RemoteDatabaseSupport.h"
#include "RemoteDatabaseConnection.h"

#include "AC_RemoteDB.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DB_CONNECTIONS_API UAC_RemoteDB : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_RemoteDB();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FRemoteDatabaseConnection* RemoteDBConnection;
	FRemoteDataBaseRecordSet* RemoteDBRecordSet;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open Remote Database", ToolTip = "Exprimental !", Keywords = "remote,database,open"), Category = "DB Connections")
	virtual bool RemoteDataBaseOpen(const FString RemoteIP, const FString ConnectionString, const FString RemoteConnectionStringOverride);

};
