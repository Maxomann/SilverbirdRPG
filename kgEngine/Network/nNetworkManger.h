//_______NETWORK_MANAGER_______//

#pragma once
#include <Algorithm/aContainer.h>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <tuple>
#include <iostream>
#include "nMessage.h"
#include "stdafx.h"

namespace kg
{

	class nNetworkManager : public pExtendable
	{
		std::map<int, std::shared_ptr<nMessageHandler>> m_messageHandler;

		sf::UdpSocket m_senderSocket;

		// 1= port
		// IPs and Socket sorted by Port
		typedef std::unordered_map<sf::Uint16, std::pair<std::vector<sf::IpAddress>, sf::UdpSocket>> ConnectionContainer;

		// senderIP, recievedPort, message(Handler)ID, message
		typedef aSwapContainer< std::tuple<sf::IpAddress, sf::Uint16, int, std::string> > MessageContainer;

		ConnectionContainer m_connectionContainer;
		MessageContainer m_messageContainer;

		static void m_networkRecieverFunction( ConnectionContainer& connections, MessageContainer& messages );

	public:
		NETWORK_API nNetworkManager();

		NETWORK_API void addConnection( sf::IpAddress& ip, sf::Uint16 port );

		NETWORK_API void sendMessage( std::shared_ptr<nMessage> message, sf::IpAddress& to, sf::Uint16 onPort );

		//the Handlers must have been added to the class by pExtension before calling this function
		NETWORK_API void initMessageHandlers();

		NETWORK_API void frame( cCore& core );
	};
}
