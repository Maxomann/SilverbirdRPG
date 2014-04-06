#include "ksFunction.h"


kg::ksFunctionMaster::ksFunctionMaster( const std::string& name )
: m_name( name )
{ }

const std::string& kg::ksFunctionMaster::getName() const
{
	return m_name;
}

void kg::ksFunctionMaster::registerOverload( const std::vector<std::string>& parameterTypes,
											 const std::shared_ptr<ksFunctionWrapperInterface>& function )
{
	//possible overloads with same signature will be overwritten
	m_boundOverloads[parameterTypes] = function;
}

void kg::ksFunctionMaster::registerOverload( const std::vector<std::string>& parameterTypes,
											 const std::shared_ptr<ksScriptFunctionOverload>& function )
{
	//possible overloads with same signature will be overwritten
	m_scriptOverloads[parameterTypes] = function;
}

void kg::ksFunctionMaster::call( std::vector<std::shared_ptr<ksClassInstance>> args ) const
{
	//get parameterTypes from args
	std::vector<std::string> parameterTypes;
	for( const auto& el : args )
		parameterTypes.push_back( el->getType() );


	//lookup for function

	//look in m_boundOverloads first
	for( const auto& el : m_boundOverloads )
	{
		if( el.first == parameterTypes )
		{
			//execute function
			//possible overloads with same signature will be ignored
			std::vector<std::shared_ptr<void>> argsConverted;
			for( const auto& obj : args )
				argsConverted.push_back( std::shared_ptr<void>( obj->getCppInstance() ) );

			el.second->call( argsConverted );
		}
	}

	//look in m_scriptOverloads
	for( const auto& el : m_scriptOverloads )
	{
		if( el.first == parameterTypes )
		{
			el.second->call( args );
		}
	}

	std::string argumentSignature;
	for( const auto& el : args )
	{
		argumentSignature += el->getType();
		argumentSignature += ",";
	}
	REPORT_ERROR_SCRIPT( "could not find overload with argumentSignature: " + argumentSignature + "in function" +m_name);

}
