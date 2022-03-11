// Copyright Epic Games, Inc. All Rights Reserved.

#include "DB_Connections.h"

#define LOCTEXT_NAMESPACE "FDB_ConnectionsModule"

void FDB_ConnectionsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FDB_ConnectionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDB_ConnectionsModule, DB_Connections)