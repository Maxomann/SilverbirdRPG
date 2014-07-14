//_______CHUNK_______//

#pragma once
#include "../stdafx.h"

namespace kg
{
	class Chunk
	{
		std::map< std::pair<int, int>, int > m_fields;

	public:
		Chunk();

		void setField( const sf::Vector2i relativeTilePosition, int id );

		int getField( const sf::Vector2i position )const;

		const std::map< std::pair<int, int>, int >& getFieldData()const;
	};
}
