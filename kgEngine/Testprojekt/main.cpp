//_______TESTPROJEKT_______//

#pragma once
#include <iostream>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/system.hpp>

#include <TGUI/TGUI.hpp>

#include <Algorithm/aCallbacks.h>

using namespace std;
using namespace sf;
using namespace tgui;
using namespace kg;

class Foo : public aCallbackSender<std::string>
{
public:
	void action(std::string str)
	{
		this->triggerCallback( 444, str );
	}
};

class Bar : public aCallbackReciever
{
	bool m_bool = false;

	void callback( const int& i, const std::string& str )
	{
		m_bool = true;
		cout << "callback:" << std::to_string( i ) << "::" << str << endl;
	};

public:
	Bar( Foo& foo )
	{
		foo.registerCallback( this,
							  std::bind( &Bar::callback,
							  this,
							  placeholders::_1,
							  placeholders::_2 ),
							  444 );
	}
};

int main()
{
	Foo foo;

	std::vector<Bar> barVec;
	barVec.emplace_back(foo);
	Bar& bar=barVec.back();


	foo.action( "Hallo" );//callback will be called

	barVec.pop_back();

	foo.action( "Hallo2" );//callback will not be called because element does not exist anymore
	
	system( "pause" );
}