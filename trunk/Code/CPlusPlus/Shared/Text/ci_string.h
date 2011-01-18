#pragma once

#include <string>

namespace Rorn
{
	namespace Text
	{
		struct ci_char_traits : public std::char_traits<char>
		{
			static bool eq( char c1, char c2 )
			{ 
				return toupper(c1) == toupper(c2); 
			}

			static bool ne( char c1, char c2 )
			{ 
				return toupper(c1) != toupper(c2); 
			}

			static bool lt( char c1, char c2 )  
			{ 
				return toupper(c1) <  toupper(c2); 
			}

			static int compare( const char* s1, const char* s2, size_t n ) 
			{
				return memicmp( s1, s2, n );
			}

			static const char* find( const char* s, int n, char a ) 
			{
				while( n-- > 0 && toupper(*s) != toupper(a) ) 
				{
					++s;
				}
      
				return s;
			}
		};

		typedef std::basic_string<char, ci_char_traits> ci_string;
	}
}