//_______MESSAGE_HANDLERS_SERVER_______//

#pragma once
#include "../stdafx.h"
#include "../Server.h"
#include "../Network/Messages.h"

namespace kg
{
	class ChunkDataRequestHandler : public nMessageHandler
	{
	public:

		virtual void handle( cCore& core, std::tuple<sf::IpAddress, sf::Uint16, int, std::string>& message ) const
		{
			auto seglist = aSplitString::function( std::get<3>( message ), standartSplitChar, aSplitString::operation::REMOVE );
			sf::Vector2i chunkPosition{ atoi( seglist.at( 0 ).c_str() ), atoi( seglist.at( 1 ).c_str() ) };

			core.networkManager.sendMessage(
				std::make_shared<ChunkDataRequestAnswer>( chunkPosition, core.getExtension<Server>()->getWorld().getChunk( chunkPosition ) ),
				std::get<0>( message ),
				//return message on port where it was recieved
				std::get<1>( message )
				);
		}

		virtual int getMessageHandlerID() const
		{
			return MESSAGE_ID_CLIENT::CHUNK_DATA_REQUEST;
		}

		virtual std::string info() const
		{
			return __CLASS__;
		}
	};

	class SetTileRequestHandler : public nMessageHandler
	{
	public:
		virtual void handle( cCore& core, std::tuple<sf::IpAddress, sf::Uint16, int, std::string>& message ) const
		{
			auto seglist = aSplitString::function( std::get<3>( message ), standartSplitChar, aSplitString::operation::REMOVE );
			sf::Vector2i chunkPosition{ atoi( seglist.at( 0 ).c_str() ), atoi( seglist.at( 1 ).c_str() ) };
			sf::Vector2i tilePosition{ atoi( seglist.at( 2 ).c_str() ), atoi( seglist.at( 3 ).c_str() ) };
			int tileID = atoi( seglist.at( 4 ).c_str() );

			auto& chunk = core.getExtension<Server>()->getWorld().getChunk( chunkPosition );
			chunk.setField( tilePosition, tileID );

			core.networkManager.spreadMessage( std::make_shared<ChunkDataRequestAnswer>( chunkPosition, chunk ) );
		}

		virtual int getMessageHandlerID() const
		{
			return MESSAGE_ID_CLIENT::SET_TILE_REQUEST;
		}

		virtual std::string info() const
		{
			return __CLASS__;
		}
	};
}
