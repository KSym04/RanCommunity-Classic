#include "StdAfx.h"
#include "./StringFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace sc 
{
    namespace string 
	{
        boost::format formatter(const std::string& f_string)
        {
            using namespace boost::io;
            boost::format fmter(f_string);
            fmter.exceptions(all_error_bits ^ (too_many_args_bit | too_few_args_bit));
            return fmter;
        }
    } // namespace sc
} // namespace string