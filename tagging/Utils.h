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
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>
#include <vector>
namespace Tagging
{
	class Utils
	{
	public:
		static void Split(std::vector<std::string> & val, const std::string &str, const std::string & seg, bool withtoken = false)
		{
			val.clear();
			std::string::size_type LeftPst = 0;
			std::string::size_type RightPst = 0;

			while ((RightPst = str.find(seg.c_str(), LeftPst)) != std::string::npos && LeftPst < str.size())
			{
				if (RightPst != 0)
				{
					std::string term;
					if (withtoken)
					{
						term = str.substr(LeftPst, RightPst - LeftPst + seg.length());
					}
					else
					{
						term = str.substr(LeftPst, RightPst - LeftPst);
					}
					if (term.length() > 0)
					{
						val.push_back(term);
					}
					LeftPst = RightPst + seg.size();
				}
				//str start with token
				else
				{
					LeftPst = RightPst + seg.size();
				}
			}
			if (LeftPst < str.size())
			{
				const std::string &term = str.substr(LeftPst);
				if (term.length() > 0)
				{
					val.push_back(term);
				}
			}
		}

		static void TrimLine(std::string &line)
		{
			line.erase(0, line.find_first_not_of(" \t\r\n"));
			line.erase(line.find_last_not_of(" \t\r\n") + 1);
		}

		static std::string ToString(int i)
		{
			std::stringstream ss;
			ss << i;
			return ss.str();
		}
	private:

	};

}
#endif
