/********************************************************************
* Copyright (C) 2011 - 2015
* The Inistitute of Chinese Ethnic Information Technology ( www.nlit.edu.cn)
* Contact: Li Yachao,harry_lyc@foxmail.com or liyc7711@gmail.com
*
* Permission to use, copy, modify, and distribute this software for
* any non-commercial purpose is hereby granted without fee, provided
* that the above copyright notice appear in all copies and that both
* that copyright notice.
* It is provided "as is" without express or implied warranty.
*
* Version: 1.6
* Last update: 2015-10-23
*********************************************************************/
#ifndef STRINGSPLIT_H
#define STRINGSPLIT_H

#include <vector>
#include <string>

namespace utility
{
	class StringSplit
	{
	public:
		static bool SplitUTF8(const std::string& utf8,std::vector<std::string>& vec);
		static bool SplitANSI(const std::string& ansi,std::vector<std::string>& vec);
		/*切分时，切分标志可以选择与前驱结合或者是不结合，不结合时丢掉*/
		static bool SplitByToken(std::vector<std::string>& vecstr, const std::string& str, const std::string token,bool withtoken = false);
		static bool SplitByTokens(std::vector<std::string>& vecstr,const std::string& str,const  std::string tokens[],const int tokensnumber,bool withtoken=false);
		/*切分标志，作为keyvalue，的value输出*/
		static bool SplitByTokens(std::vector<std::pair<std::string,std::string> > & vecstr,const std::string& str,const  std::string tokens[],const int tokensnumber);
		/*切分时，切分标志单独切分出来*/
		static bool SplitToken(std::vector<std::string> &vecstr, const std::string &str, const std::string token);
		/*切分时，切分标志单独切分出来*/
		static bool SplitTokens(std::vector<std::string> &vecstr, const std::string &str, const std::string tokens[], const int tokensnumber);
	private:
	};
}

#endif
