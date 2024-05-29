// Fill out your copyright notice in the Description page of Project Settings.

#include "RemoteJumpComponent.h"
#include "MessagingServer.h"
#include "MessageEndpointBuilder.h"

// Sets default values for this component's properties
URemoteJumpComponent::URemoteJumpComponent()
{
}

// Called when the game starts
void URemoteJumpComponent::BeginPlay()
{
	Super::BeginPlay();

	// initialize an endpoint that waits for the the jump message
	JumpEndpoint = FMessageEndpoint::Builder("FRemoteJumpComponent")
		.Handling<FJumpNowMessage>(this, &URemoteJumpComponent::JumpNowHandler);

	// if the endpoint is valid, we successfully connected to the message bus.
	// this means we can subscribe to the broadcast messages we want to be notified of.
	if (JumpEndpoint.IsValid())
		JumpEndpoint->Subscribe<FJumpNowMessage>();
}

// Called when the game ends
void URemoteJumpComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// release the message endpoint, which disconnects from the message bus.
	JumpEndpoint.Reset();

	Super::EndPlay(EndPlayReason);
}

void URemoteJumpComponent::JumpNowHandler(const FJumpNowMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	// notify the blueprint that someone wants us to jump
	OnJumpNow.Broadcast();
}
