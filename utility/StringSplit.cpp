#include "StringSplit.h"

namespace utility
{
	/*
    *************************************************
    功能   :切分UTF8编码格式的字节流为字符串数组
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :
    -------------------------------------------------
	作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	bool StringSplit::SplitUTF8(const std::string &utf8, std::vector<std::string> &vec)
	{
		vec.clear();
		for(int i=0;i<utf8.length();)
		{
			std::string myChar = utf8.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			if (myCh1 < 128)
			{
				i ++;
			}
			else if (myCh1 < 224)
			{
				myChar = utf8.substr(i, 2);
				i += 2;
			}
			else if (myCh1 < 240)
			{
				myChar = utf8.substr(i, 3);
				i += 3;
			}
			else
			{
				myChar = utf8.substr(i, 4);
				i += 4;
			}
			vec.push_back(myChar);
		}
		return true;
	}
	bool StringSplit::SplitANSI(const std::string& ansi, std::vector<std::string>& vec)
	{
		vec.clear();
		for(int i=0;i<ansi.size();i++)
		{
			std::string myChar = ansi.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			if (myCh1 < 128)
			{
				//i ++;
			}
			else
			{
				myChar = ansi.substr(i, 2);
				i += 1;
			}
			vec.push_back(myChar);
		}
		return true;
	}
	/*
    *************************************************
    功能   :将字符串按照一定的Token进行切分
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :withtoken为true切分结果包含token，false不包含token，默认值为false
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-28
    *************************************************
	*/
	bool StringSplit::SplitByToken(std::vector<std::string> &vecstr, const std::string &str, const std::string token,bool withtoken)
	{
		vecstr.clear();
		std::string::size_type LeftPst = 0;
		std::string::size_type RightPst = 0;

		while((RightPst = str.find(token.c_str(), LeftPst)) != std::string::npos && LeftPst < str.size())
		{
			if(RightPst != 0)
			{
				std::string term ;
				if(withtoken)
				{
					term = str.substr(LeftPst, RightPst-LeftPst + token.length());
				}
				else
				{
					term = str.substr(LeftPst, RightPst-LeftPst);
				}
				if( term.length() > 0 )
				{
					vecstr.push_back(term);
				}
				LeftPst = RightPst + token.size();
			}
			//str以token开头
			else
			{
				LeftPst = RightPst + token.size();
			}
		}
		if(LeftPst < str.size())
		{
			const std::string &term = str.substr(LeftPst);
			if( term.length() > 0 )
			{
				vecstr.push_back(term);
			}
		}
		return (!vecstr.empty());
	}
	/*
    *************************************************
    功能   :将字符串按照一定的Tokena进行切分
    参数   :vecstr切分结果列表，str要切分的字符串，tokens[]切分标志，tokensnumber tokens数量，结果带不带token
    返回值 :
    -------------------------------------------------
    备注   :withtoken为true切分结果包含token，false不包含token，默认值为false
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-28
    *************************************************
	*/
	bool StringSplit::SplitByTokens(std::vector<std::string> &vecstr, const std::string &str, const std::string tokens[],const  int tokensnumber, bool withtoken)
	{
		vecstr.clear();
		if((str.empty()) ||tokensnumber <=0 )
		{
			return false ;
		}
		std::string buffer="";
		int textLength = str.length();
		int start = 0;
		int offset = 0;
		while(start < textLength)
		{
			offset = textLength;
			int subLength =0;
			std::string tmp ="";
			for(int i=0;i< tokensnumber;i++)
			{
				if(tokens[i].empty())
				{
					continue;
				}
				int curr = str.find(tokens[i],start);
				if((curr >= 0) &&(curr < offset))
				{
					offset = curr;
					subLength  = tokens[i].length();

				}
			}
			if(start == offset)
			{
				if(withtoken)
				{
					tmp = str.substr(start,subLength);
				}
				else
				{
					tmp = str.substr(start,0);
				}
				start = offset + subLength;
			}
			else if(start < offset)
			{
				int len = 0;
				if(withtoken)
				{
					len = subLength + ( offset - start);
				}
				else
				{
					len = ( offset - start);
				}
				tmp = str.substr(start,len);
				start = (offset + subLength  );
			}
			/*这个影响多个空格连在一块，并且切分标志位空格的情况*/
			if(!tmp.empty())
			{
				vecstr.push_back(tmp);
			}
		}
		return true ;
	}
	/*
    *************************************************
    功能   :将字符串按照一定的Token进行切分，结果返回词和词性对。
    参数   :vecstr切分结果列表，str要切分的字符串，tokens[]切分标志，tokensnumber tokens数量
    返回值 :
    -------------------------------------------------
    备注   :withtoken为true切分结果包含token，false不包含token，默认值为false
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	bool StringSplit::SplitByTokens(std::vector<std::pair<std::string,std::string> > &vecstr, const std::string &str, const std::string tokens[], const int tokensnumber)
	{
		vecstr.clear();
		if((str.empty()) ||tokensnumber <=0 )
		{
			return false ;
		}
		std::string buffer="";
		int textLength = str.length();
		int start = 0;
		int offset = 0;
		while(start < textLength)
		{
			offset = textLength;
			int subLength =0;
			std::string tmp ="";
			std::pair<std::string,std::string> kv;
			for(int i=0;i< tokensnumber;i++)
			{
				if(tokens[i].empty())
				{
					continue;
				}
				int curr = str.find(tokens[i],start);
				if((curr >= 0) &&(curr < offset))
				{
					offset = curr;
					subLength  = tokens[i].length();

				}
			}
			if(start == offset)
			{
				tmp = str.substr(start,0);
				start = offset + subLength;
			}
			else if(start < offset)
			{
				int len = 0;
				/*if(withtoken)
				{
					len = subLength + ( offset - start);
				}
				else
				{
					len = ( offset - start);
				}*/
				tmp = str.substr(start,len);
				//std::string key = str.substr(start,offset - start);
				//std::string value = str.substr(offset,subLength);
				kv.first = str.substr(start,offset - start);
				kv.second = str.substr(offset,subLength);
				//kv.key = key ;
				//kv.value = value ;

				start = (offset + subLength  );
			}
			/*这个影响多个空格连在一块，并且切分标志位空格的情况*/
			if(!kv.first.empty())
			{
				vecstr.push_back(kv);
			}
		}
		return true ;
	}
	/*
    *************************************************
    功能   :将字符串按照一定的Token进行切分,Token单独切开
    参数   :vecstr切分结果列表，str要切分的字符串，tokens
    返回值 :
    -------------------------------------------------
    备注   :例子"ab/c","abc","/","c"
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-12-14
    *************************************************
	*/
	bool StringSplit::SplitToken(std::vector<std::string> &vecstr, const std::string &str, const std::string token)
	{
		vecstr.clear();
		int start = 0;
		int offset = 0;
		std::string term ="";
		std::string tk ="";
		while(true)
		{
			offset  = str.find(token,start);
			if((offset < 0) || (offset < start))
			{
				if((offset < 0) && (start == 0))
				{
					vecstr.push_back(str);
				}
				else if((offset < 0) && (start > 0))
				{
					term = str.substr(start,str.size() - start);
					if(!term.empty())
					{
						vecstr.push_back(term);
					}
				}
				break;
			}
			term = str.substr(start,offset - start);
			if(offset + token.size() <= str.size())
			{
				tk = str.substr(offset , token.size());
			}
			//TrimAll(term);
			if(!term.empty())
			{
				vecstr.push_back(term);
			}
			if(!tk.empty())
			{
				vecstr.push_back(tk);
			}
			start = offset + token.size() ;
		}
		return true;
	}

	bool StringSplit::SplitTokens(std::vector<std::string> &vecstr, const std::string &str, const std::string tokens[], const int tokensnumber)
	{
		vecstr.clear();
		if((str.empty()) ||tokensnumber <=0 )
		{
			return false ;
		}
		std::string buffer="";
		int textLength = str.length();
		int start = 0;
		int offset = 0;
		while(start < textLength)
		{
			offset = textLength;
			int subLength =0;
			std::string tmp ="";
			std::pair<std::string,std::string> kv;
			for(int i=0;i< tokensnumber;i++)
			{
				if(tokens[i].empty())
				{
					continue;
				}
				int curr = str.find(tokens[i],start);
				if((curr >= 0) &&(curr < offset))
				{
					offset = curr;
					subLength  = tokens[i].length();
				}
			}
			if(start == offset)
			{
				tmp = str.substr(start,0);
				start = offset + subLength;
			}
			else if(start < offset)
			{
				int len = 0;
				tmp = str.substr(start,len);
				std::string key = str.substr(start,offset - start);
				std::string value = str.substr(offset,subLength);
				kv.first = key ;
				kv.second = value ;
				start = (offset + subLength  );
			}
			/*这个影响多个空格连在一块，并且切分标志位空格的情况*/
			if(!kv.first.empty())
			{
				//vecstr.push_back(kv);
				vecstr.push_back(kv.first);
				vecstr.push_back(kv.second);
			}
		}
		return true ;
	}

}
