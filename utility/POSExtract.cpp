#include "POSExtract.h"

namespace utility
{
	/*
    *************************************************
    功能   :解析标记的文本，标记的文本没有空格作为区分标志
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :标注语料抽取需要根据具体语料而定，这个是通用的抽取方法，不考虑特殊情况
    -------------------------------------------------
	作者   ：Li Yachao
    时间   ：2013-3-6
    *************************************************
	*/
	void POSExtract::ParsePOS(const std::string& line, std::vector<std::pair<std::string, std::string> >& tokenList)
	{
		tokenList.clear();
		if(line.empty())
		{
			return ;
		}
		std::string segtag = "/";
		int segpos         = 0;
		int offset         = 0;
		int index          = 0;
		while(segpos<line.size())
		{
			segpos = line.find(segtag,offset);
			if((segpos < 0) || (segpos < offset ))
			{
				break;
			}
			char prefix = '-';/*限制标记混合，字母+数字类型导致的错误*/
			while(true)
			{
				if(segpos + segtag.size() + index>= line.size())
				{
					break;
				}
				unsigned char c = line.at(segpos + segtag.size() + index);
				if(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9')))
				{
					if(((c >= 'a') && (c <= 'z')))
					{
						if(prefix == '-')
						prefix = c;
						if((prefix == 'p'))
						{
							int sa= 0;
						}
						index++;
					}
					else if((prefix == 'p') && (c >= '0') && (c <= '9'))
					{
						index++;
						break;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			std::pair<std::string,std::string> kv;
			kv.first  = line.substr(offset, segpos - offset);
			kv.second = line.substr(segpos + segtag.size() ,index);
			offset    = segpos + segtag.size() + index;
			index     = 0;
			TrimAll(kv.first);
			if(!kv.first.empty())
			{
				tokenList.push_back(kv);
			}
		}
	}
	void POSExtract::TrimAll(std::string &str)
	{
		str.erase(0,str.find_first_not_of(" \t\r\n"));
		str.erase(str.find_last_not_of(" \t\r\n")+1);
	}
}
