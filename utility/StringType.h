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
#ifndef STRINGTYPE_H
#define STRINGTYPE_H
#include <string>
#include <vector>
#include <set>

#include "StringSplit.h"

namespace utility
{
	class StringType
	{
	public:
		StringType();
		bool Init();
		int CH_GBKCharType(const std::string& myChar);

		static int TB_UTF8CharType(const std::string & myChar);
		static int TB_UTF8StringType(const std::string& str);
		static int UTF16CharType(const std::string & str);
		static bool IsChGBKPunctuation(const std::string& str);
		/*是否为ASCII编码字符串*/
		static bool IsASCIIString(const std::string& word);
		/*是否为中文数字*/
		static bool IsCHGBKNumber(const std::string& str);
		/*是否是中文字符*/
		static bool IsCHBGKChar(const std::string& str);
		static int CHGBKCharIndex(const std::string& chChar);
	private:
		/*为啥static类型的出错呢？*/
		std::set<unsigned int> gbktable;
	};
}
#endif
