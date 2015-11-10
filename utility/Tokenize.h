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
/*
对输入的文本进行序列化预处理
*/
#ifndef TOKENNIZE_H
#define TOKENNIZE_H
#include <vector>
#include <list>
#include "StringSplit.h"
#include "StringOperation.h"
#include "TextClassification.h"

namespace utility
{
	class Tokenize
	{
	public:
		Tokenize();
		~Tokenize();
		/*中文预处理，以UTF8编码的文本文件读取的行*/
		void        GBK(const std::string& line, std::vector<std::pair<std::string,std::string> >& val);
		void        GBKAll(const std::string& line, std::vector<std::pair<std::string,std::string> >& val);
		static void Chinese_UTF8(const std::string& line, std::vector<std::string>& vec_val);
		/*中文预处理，以ANSI编码的文本文件读取的行*/
		static void Chinese_ANSI(const std::string& line, std::vector<std::string>& vec_val);
		static void Chinese_ANSIALL(const std::string& line, std::vector<std::string>& vec_val);
		static void Chinese_ANSI(const std::string& line, std::list<std::string>& vec_val);
		/*藏文预处理，以UTF8编码的文本文件读取的行，按照音节点进行切分，格助词不予处理*/
		static void Tibetan(const std::string & line,std::vector<std::string>& val);
		static void Tibetan(const std::string & line,std::vector<std::pair<std::string,std::string> >& val);
		static void Tibetan(const std::string & line,std::list<std::string>& val);
		/*藏文预处理，以UTF8编码的文本文件读取的行，按照音节点进行切分，对于格助词予以全部切分*/
		static void TibetanAll(const std::string & line,std::vector<std::string>& val);
		static void TibetanAll(const std::string & line,std::vector<std::pair<std::string,std::string> >& val);
		static void TibetanAll(const std::string & line,std::list<std::string>& val);
	private:
		StringType* str_type;
	};
}
#endif 
