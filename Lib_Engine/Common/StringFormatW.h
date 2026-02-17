#ifndef SC_STRING_FORMATW_H_
#define SC_STRING_FORMATW_H_

#pragma once

#include <iostream>
#include <string>
#pragma warning(disable:4996) // disable deprecated warning 
#include <boost/format.hpp>
#pragma warning(default:4996)
#include <boost/preprocessor.hpp>
#include <boost/algorithm/string.hpp>

namespace sc 
{
    namespace string 
	{
        boost::wformat wformatter( const std::wstring& f_string );        

#define X_GENERATE_FORMAT_W(z,n,x) %arg##n

#define X_GENERATE_FORMAT_FUNCTION_W(z,n,d)                                                  \
	template< BOOST_PP_ENUM_PARAMS(n,typename T)>											\
	std::wstring format( const std::wstring& strFmt, BOOST_PP_ENUM_BINARY_PARAMS(n, T, arg) )	\
	try																						\
		{																					\
		using namespace boost::io;															\
		boost::format f;																	\
		f.exceptions( all_error_bits ^ ( too_many_args_bit | too_few_args_bit )  );			\
		f.parse(strFmt);																	\
		f BOOST_PP_REPEAT(n,X_GENERATE_FORMAT_W,_);                                          \
		return f.str();																		\
		}																					\
		catch(boost::io::format_error& ex)													\
		{																					\
		std::wstring Err(strFmt);															\
		Err.append(" ");																	\
		Err.append(ex.what());																\
		return Err;																			\
		}

		BOOST_PP_REPEAT_FROM_TO( 1, 50, X_GENERATE_FORMAT_FUNCTION_W, _)

#undef X_GENERATE_FORMAT_FUNCTION_W
#undef X_GENERATE_FORMAT_W

    } // namespace string
} // namespace sc

#define wstringformat sc::string::wformat

#endif // _CORE_COMMON_STRING_FORMATW_H_