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

#ifndef TEXTCLASSIFICATION_H
#define TEXTCLASSIFICATION_H
#include <vector>
#include <string>
#include <sstream>
#include "StringSplit.h"
#include "StringType.h"
#include "StringOperation.h"

namespace utility
{
	class TextClassification
	{
	public:
		TextClassification();
		~TextClassification();
		static bool TB_UTF8(const std::string& text,std::vector<std::pair<std::string,std::string> >& vec);
		bool        CH_GBK(const std::string& text,std::vector<std::pair<std::string,std::string> >& vec);
	private:
		StringType * gbk_type;
	};
}
#endif
