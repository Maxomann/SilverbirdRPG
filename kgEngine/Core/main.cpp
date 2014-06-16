//_______MAIN_______//

#pragma once
#include "stdafx.h"
#include <iostream>
#include "cCore.h"
#include "cCoreExtension.h"

// Interface classes:
// pCoreExtension
// nMessage
// nMessageHandler

using namespace std;
using namespace kg;


int main()
{
	try
	{
		pPluginManager pluginManager;
		pluginManager.loadPluginsFromFile( "Client.dll" );
		pluginManager.loadPluginsFromFile( "Server.dll" );

		nNetworkManager networkManger;
		pluginManager.fillExtandable<nNetworkManager>( networkManger );
		networkManger.initMessageHandlers();

		networkManger.addConnection( sf::IpAddress::getLocalAddress(), 42000 );

		cCore application;
		pluginManager.fillExtandable<cCore>( application );

		//Main loop
		while( !application.shouldClose() )
		{
			networkManger.frame( application );
			application.frame( networkManger );
		}
	}
	catch( std::exception& e )
	{
		cout << e.what() << endl;
		system( "pause" );
	}

	return 0;
}
