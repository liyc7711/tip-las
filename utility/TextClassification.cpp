#include "TextClassification.h"

namespace utility
{
	TextClassification::TextClassification()
	{
		gbk_type = new StringType();
	}
	TextClassification::~TextClassification()
	{
		delete gbk_type;
	}
	/*
    *************************************************
    功能   :对UTF8编码藏文文件进行分类，藏文，标点符号，英文等
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2013-2-25
    *************************************************
	*/
	bool TextClassification::TB_UTF8(const std::string &text, std::vector<std::pair<std::string,std::string> > & vec)
	{
		vec.clear();
		if(text.empty())
		{
			return true;
		}
		//TrimAll(text);
		std::vector<std::string>charList;
		StringSplit::SplitUTF8(text,charList);
		int currentType = StringType::TB_UTF8CharType(charList[0]);
		int priorType = currentType ;
		std::string buffer="";
		//char bf[3]={'\0'};
		for(int i=0;i<charList.size();i++)
		{
			std::string myChar = charList[i];
			currentType = StringType::TB_UTF8CharType(myChar);
			if(currentType != priorType)
			{
				StringOperation::TrimAll(buffer);
				if(!buffer.empty())
				{
					std::stringstream ss;
					ss << priorType;
					std::string bf = ss.str();
					std::pair<std::string,std::string> kv;
					kv.second = bf;
					kv.first= buffer;
					vec.push_back(kv);
				}
				buffer.clear();
			}
			buffer +=myChar;
			priorType = currentType;
		}
		StringOperation::TrimAll(buffer);
		if(!buffer.empty())
		{
			std::pair<std::string,std::string> kv;
			std::stringstream ss;
			ss << priorType;
			std::string bf = ss.str();
			kv.second = bf;
			kv.first = buffer;
			vec.push_back(kv);
		}
		buffer.clear();
		return true;
	}

	/*
    *************************************************
    功能   :对GBK编码文件进行分类，中文，标点符号，英文等
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :1表示汉字；2如标点符号；3表示gbk图形符号；4表示英文等；5表示未知
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2013-2-25
    *************************************************
	*/
	bool TextClassification::CH_GBK(const std::string &text, std::vector<std::pair<std::string,std::string> > &vec)
	{
		vec.clear();
		if(text.empty())
		{
			return true;
		}
		std::vector<std::string>charList;
		StringSplit::SplitANSI(text,charList);
		int currentType = gbk_type->CH_GBKCharType(charList[0]);
		int priorType = currentType ;
		std::string buffer="";
		for(int i=0;i<charList.size();i++)
		{
			std::string myChar = charList[i];
			currentType = gbk_type->CH_GBKCharType(myChar);
			if(currentType != priorType)
			{
				StringOperation::TrimAll(buffer);
				if(!buffer.empty())
				{
					std::stringstream ss;
					ss << priorType;
					std::string bf = ss.str();
					std::pair<std::string,std::string> kv;
					kv.second = bf;
					kv.first= buffer;
					vec.push_back(kv);
				}
				buffer.clear();
			}
			buffer +=myChar;
			priorType = currentType;
		}
		StringOperation::TrimAll(buffer);
		if(!buffer.empty())
		{
			std::pair<std::string,std::string> kv;
			std::stringstream ss;
			ss << priorType;
			std::string bf = ss.str();
			kv.second = bf;
			kv.first = buffer;
			vec.push_back(kv);
		}
		buffer.clear();
		return true;
	}
}
