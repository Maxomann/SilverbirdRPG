#include "../Client.h"

namespace kg
{
	const char kg::Client::config_file_path[] = "config_client.txt";

	kg::Client::Client()
		:m_gui( m_window )
	{ }

	void Client::onInit( cCore& core )
	{
		auto database = core.getExtension<ClientDatabase>();
		database->loadAllResources( core );

		m_world.onInit( core );

		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = database->getAntialiasingLevel();

		//initialize the Client with the data from the config_file
		m_window.create( sf::VideoMode( database->getWindowResolution().x, database->getWindowResolution().y ),
						 database->getWindowName(),
						 sf::Style::Titlebar | sf::Style::Close,
						 contextSettings );
		//camera
		m_camera.init( sf::Vector2u( database->getWindowResolution() ) );
		//vSynch
		m_window.setVerticalSyncEnabled( database->isVsynchEnabled() );
		//renderDistance
		sf::Vector2i renderDistance = database->getRenderDistance();
		m_renderDistaceInChunks.left = -renderDistance.x*chunkSizeInTiles*tileSizeInPixel;
		m_renderDistaceInChunks.top = -renderDistance.y*chunkSizeInTiles*tileSizeInPixel;
		m_renderDistaceInChunks.width = renderDistance.x * 2 * chunkSizeInTiles*tileSizeInPixel;
		m_renderDistaceInChunks.height = renderDistance.y * 2 * chunkSizeInTiles*tileSizeInPixel;

		//init GUI
		if( !m_gui.setGlobalFont( resourceFolderPath + fontFolderName + "DejaVuSans.ttf" ) )
			REPORT_ERROR_FILEACCESS( resourceFolderPath + fontFolderName + "DejaVuSans.ttf" + "could not be loaded" );
	}

	void kg::Client::frame( cCore& core )
	{
		auto frameTime = m_frameTimeClock.restart().asMilliseconds();
		m_window.setTitle( core.getExtension<ClientDatabase>()->getWindowName() + " --- FrameTime: " + std::to_string( frameTime ) );

		if( !m_isStandartGameStateLoaded )
		{
			//set standart gameState
			m_gameState = m_gameStates.at( GAME_STATE_ID::STANDART );
			m_gameState->onInit( core, m_world, m_camera, m_gui );
			m_isStandartGameStateLoaded = true;
		}

		//SFML loop:
		sf::Event event;
		while( m_window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
				core.close();

			m_gui.handleEvent( event );
			m_gameState->handleEvent( event );
		}
		
		// change gameState if needed
		int newGameStateId = m_gameState->frame( core, m_window, m_world, m_camera, m_gui );
		if( newGameStateId == GameState::CLOSE_APP )
			core.close();
		else if( newGameStateId > GameState::NO_CHANGE )
		{
			m_gameState->onClose( core, m_world, m_camera, m_gui );
			m_gameState = m_gameStates.at( newGameStateId );
		}

		//apply render distance; unload all chunks that are not in it
		auto chunkRenderRect = m_renderDistaceInChunks;
		auto offset = m_camera.getCenter();
		chunkRenderRect.left += offset.x;
		chunkRenderRect.top += offset.y;
		m_world.loadChunksInRectAndUnloadOther( core, { chunkRenderRect } );

		//Call frame() here:
		m_world.frame( core );

		//Camera drawing here:
		m_world.draw( m_camera );

		m_window.clear( sf::Color::Green );
		//window-drawing here:
		m_camera.display( m_window );
		m_gui.draw();
		m_window.display();
	}

	std::string kg::Client::info() const
	{
		return "Build-in CLIENT plugin";
	}

	kg::World& kg::Client::getWorld()
	{
		return m_world;
	}

	void Client::initExtensions( pPluginManager& pluginManager )
	{
		pluginManager.fillExtandable<Client>( *this );

		for( const auto& el : m_extensions )
		{
			auto ptr = std::dynamic_pointer_cast< GameState >(el.second);
			if( ptr )
			{
				m_gameStates[ptr->getID()] = ptr;
			}
		}
	}

	void Client::onClose( cCore& core )
	{
		core.getExtension<ClientDatabase>()->saveAllResources();
	}
}