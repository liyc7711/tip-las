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
/********************************************************************
* Thanks to Wang Kun and Li Xiaoqing for the help of this project.
*
*
********************************************************************/

#ifndef FORMATTING_H
#define FORMATTING_H
#include <string>
#include <vector>
#include "./utility/head.h"
using namespace utility;

namespace utility_train
{
	class Formatting
	{
	public:
		Formatting();
		~Formatting();
		void WsTrain(const std::string& text, std::vector<std::vector<std::string> >& features, const std::string& language = "TB", const std::string& encode = "UTF8");
		void WsTest(const std::string& text, std::vector<std::vector<std::string> >& features, std::vector<bool>& seg,
			        const std::string& language ="TB" ,const std::string& encode="UTF8");
		void PosTrain(const std::string& text, std::vector<std::vector<std::string> >& features);
		void PosTest(const std::string& text, std::vector<std::vector<std::string> >& features, std::vector<bool>& seg);
		void PosTrainS(const std::string& text, std::vector<std::vector<std::string> >& features);
		void PosTestS(const std::string& text, std::vector<std::vector<std::string> >& features, std::vector<bool>& seg);
	private:
		Tokenize*   tokenize;
		std::string delimiter;/**/
		std::string head;
		std::string tail;
		void WSTag(const std::string& word,std::vector<std::pair<std::string,std::string> >& tags, const std::string& language = "TB",const std::string& code="UTF8");
	};
}

#endif
