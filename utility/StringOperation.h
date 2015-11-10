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
#ifndef STRINGOPERATION_H
#define STRINGOPERATION_H

#include <string>
namespace utility
{
	class StringOperation
	{
	public:
		static bool IsPrefix(const std::string& src, const std::string& prefix);
		static bool IsPostfix(const std::string& src, const std::string& postfix);
		static void TrimAll(std::string& str);  
		static void TrimBlanks(std::string& str);
		static void TrimBlanksBegin(std::string& str);
		static void TrimBlanksEnd(std::string& str);
		/*一个字符串是否包含另外一个字符串******/
		static bool Contains(const std::string src,const std::string obj);
		static int  MatchingNumber(const std::string& src,const std::string& obj);
		/*把字符串按照一定的标识符切割成字符串对*/
		static void String2Pair(const std::string& str,std::pair<std::string,std::string>& kv,const std::string& seg="/");
		static void Replace(std::string& text,const std::string& old_str,const std::string& new_str);
		static void Remove(std::string& text,const std::string& remove_str);
		static void Remove(std::string& text,const std::string  remove_strs[],int remove_count);
	private:
	};
}
#endif
