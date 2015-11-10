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
* Last update: 2015-10-22
*********************************************************************/
#ifndef POSEXTRACT_H
#define POSEXTRACT_H
#include <vector>
#include <string>

namespace utility
{
	class POSExtract
	{
	public:
		static void ParsePOS(const std::string& line, std::vector<std::pair<std::string,std::string> >& tokenList);
		static void TrimAll(std::string &str);
	private:
	};
}
#endif
