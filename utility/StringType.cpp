#include "StringType.h"

namespace utility
{
	StringType::StringType()
	{
		Init();
	}
	bool StringType::Init()
	{
		/*for(int i=0;i<21003;i++)
		{
			gbktable.insert(gbk_table[i]);
		}*/
		return true;
	}
	/*  
    *************************************************
    功能   :取得一个UTF字符的类型;
    参数   : 
    返回值 :
    -------------------------------------------------
    备注   :
    -------------------------------------------------
	作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	int StringType::TB_UTF8CharType(const std::string &myChar)
	{
		int val = -1;
		if(myChar.length() == 1)
		{
			unsigned short c = myChar[0];
			if(((c >= 97) && (c <= 122)) || ((c >= 65) && (c <= 90)))
			{
				val = 0;/*英文字母，26个，大小写*/
			}
			else if( (c >= 48) && ( c <= 57 ) )
			{
				val = 1;/*英文数字*/
			}
			else if(c == '.')
			{
				val = 1;/*英文数字，解决小数点切分错误*/
			}
			else if( ((c >= '\x1')&& (c <= '/')) ||( (c >= ':') && (c <= '@') ) || ( ( c >= '[') && (c <= '~') )  )
			{
				val = 2;/*英文符号*/
			}
			else
			{
				val = 2;
				return val;
			}
		}
		else if(myChar.length() == 3)
		{	/*
			std::string str="。，、；：
			？！“”‘’
			╗╚（）…—
			《》〈〉•";
			*/
			const int length = 22;
			std::string chCharList[length] = {"\xe3\x80\x82","\xef\xbc\x8c","\xe3\x80\x81","\xef\xbc\x9b","\xef\xbc\x9a",\
				"\xef\xbc\x9f","\xef\xbc\x81","\xe2\x80\x9c","\xe2\x80\x9d","\xe2\x80\x98","\xe2\x80\x99",\
				"\xe2\x95\x97","\xe2\x95\x9a","\xef\xbc\x88","\xef\xbc\x89","\xe2\x80\xa6","\xe2\x80\x94",\
				"\xe3\x80\x8a","\xe3\x80\x8b","\xe3\x80\x88","\xe3\x80\x89","\xe2\x80\xa2"};
			for(int i=0;i<length;i++)
			{
				if(myChar == chCharList[i])
				{
					val = 3;/*汉语标点符号*/
					break;
				}
			}
			if(val != 3)
			{
				val = 4;/*默认为汉语或者是藏语字符*/
			}
		}
		else
		{
			return val;
		}
		return val;
	}
	int StringType::TB_UTF8StringType(const std::string &str)
	{
		int firstType = 5 ;
		int currentType = firstType;
		const int length = 22;
		std::vector<std::string> utf8chars;
		std::string chCharList[length] = {"\xe3\x80\x82","\xef\xbc\x8c","\xe3\x80\x81","\xef\xbc\x9b","\xef\xbc\x9a",\
			"\xef\xbc\x9f","\xef\xbc\x81","\xe2\x80\x9c","\xe2\x80\x9d","\xe2\x80\x98","\xe2\x80\x99",\
			"\xe2\x95\x97","\xe2\x95\x9a","\xef\xbc\x88","\xef\xbc\x89","\xe2\x80\xa6","\xe2\x80\x94",\
			"\xe3\x80\x8a","\xe3\x80\x8b","\xe3\x80\x88","\xe3\x80\x89","\xe2\x80\xa2"};
		StringSplit::SplitUTF8(str,utf8chars);
		for(int i=0;i< (utf8chars.size());i++)
		{
			if((utf8chars[i].length() == 1))
			{
				char c = utf8chars[i].at(0);
				if(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
				{
					currentType = 1;/*英文字母，26个，大小写*/
				}
				else if( (c >= '0') && ( c <= '9' ) )
				{
					currentType = 2;/*英文数字*/
				}
				else if((c == '.'))
				{
					if((i > 0) && (utf8chars[i-1] != "."))
					{
						currentType = 2;/*英文数字，解决小数点切分错误*/
					}
					else if((i +1 < utf8chars.size()) && (utf8chars[i+1] !="."))
					{
						currentType = 2 ;
					}
					else
					{
						currentType = 3 ;
					}
				}
				else if( ((c >= '\x1')&& (c <= '/')) ||( (c >= ':') && (c <= '@') ) || ( ( c >= '[') && (c <= '~') )  )
				{
					currentType = 3;/*英文符号*/
				}
			}
			else
			{
				bool t = true ;
				for(int j=0;j<length;j++)
				{	
					if(utf8chars[i] == chCharList[j])
					{
						currentType = 4;/*汉语标点符号*/
						t = false;
						break;
					}
				}
				if(t)
				{
					currentType = 5 ;
				}
				
			}
			if(i>0)
			{
				if(currentType != firstType)
				{
					return 0;
				}
			}
			else
			{
				firstType = currentType;
			}
		}
		return firstType;
	}
	/*  
    *************************************************
    功能   :判断GBK编码字符串类型
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :1表示汉字；2如标点符号；3表示gbk图形符号；4表示英文等；5表示未知
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2013-2-25
    *************************************************
	*/
	int StringType::CH_GBKCharType(const std::string&  myChar)
	{
		int val = 1;/*汉字，不包括标点符号*/
		if(myChar.size() == 1)
		{
			val = 4;
			/*if(IsASCIIString(myChar))
			{
				val = 3;
			}*/
		}
		else if(myChar.size() == 2)
		{
			unsigned char c0 = myChar.at(0);
			unsigned char c1 = myChar.at(1);
			unsigned int key = c0 << 8;
			key += c1;
			std::set<unsigned int>::iterator it = gbktable.find(key);
			if(it != gbktable.end())
			{
				val = 1;
			}
			else
			{
				if(IsChGBKPunctuation(myChar))
				{
					val = 2;
				}
				else
				{
					val = 3;
				}
			}
			
		}
		else
		{
			val = 4;
		}

		return val;
	}
	/*  
    *************************************************
    功能   :判断字符串是否为ASCII编码字符串
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :判断输入的字符串是否全部是英文字符
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-28
    *************************************************
	*/
	bool StringType::IsASCIIString(const std::string& word)
	{
		for (unsigned int i = 0; i < word.size(); i++)
		{
			if (word[i] < 0)
			{
				return false;
			}
		}
		return true;
	}
	/*  
    *************************************************
    功能   :判断字符串是否为中文数字
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :一二三四五六七八九十零〇千万亿
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2013-2-25
    *************************************************
	*/
	bool StringType::IsCHGBKNumber(const std::string& str)
	{
		bool val = false;
		if(str == "\xd2\xbb")
		{/*一*/
			val = true;
		}
		else if(str == "\xb6\xfe")
		{
			val = true;
		}
		else if(str == "\xc8\xfd")
		{
			val = true;
		}
		else if(str == "\xcb\xc4")
		{
			val = true;
		}
		else if(str == "\xce\xe5")
		{
			val = true;
		}
		else if(str == "\xc1\xf9")
		{
			val = true;
		}
		else if(str == "\xc6\xdf")
		{
			val = true;
		}
		else if(str == "\xb0\xcb")
		{
			val = true;
		}
		else if(str == "\xbe\xc5")
		{
			val = true;
		}
		else if(str == "\xca\xae")
		{
			val = true;
		}
		else if(str == "\xc1\xe3")
		{
			val = true;
		}
		else if(str == "\xa9\x96")
		{
			val = true;
		}
		else if(str == "\xc7\xa7")
		{
			val = true;
		}
		else if(str == "\xcd\xf2")
		{
			val = true;
		}
		else if(str == "\xd2\xda")
		{
			val = true;
		}
		return val;
		
		//return true;
	}

	/*  
    *************************************************
    功能   :判断字符串是否为ASCII编码标点符号
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :。，、；：？！“”‘’╗╚（）…—《》〈〉【】
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2013-2-25
    *************************************************
	*/
	bool StringType::IsChGBKPunctuation(const std::string& str)
	{
		bool val = false;
		if(str == "\xa1\xa3")
		{
			val = true;
		}
		else if(str == "\xa3\xac")
		{
			val = true;
		}
		else if(str == "\xa1\xa2")
		{
			val = true;
		}
		else if(str == "\xa3\xbb")
		{
			val = true;
		}
		else if(str == "\xa3\xba")
		{
			val = true;
		}
		else if(str == "\a3\xbf")
		{
			val = true;
		}
		else if(str == "\xa3\xa1")
		{
			val = true;
		}
		else if(str == "\xa1\xb0")
		{
			val = true;
		}
		else if(str == "\xa1\xb1")
		{
			val = true;
		}
		else if(str == "\xa1\xae")
		{
			val = true;
		}
		else if(str == "\xa1\xaf")
		{
			val = true;
		}
		else if(str == "\xa8\x5b")
		{
			val = true;
		}
		else if(str == "\xa8\x5e")
		{
			val = true;
		}
		else if(str == "\xa3\xa8")
		{
			val = true;
		}
		else if(str == "\xa3\xa9")
		{
			val = true;
		}
		else if(str == "\xa1\xad")
		{
			val = true;
		}
		else if(str == "\xa1\xaa")
		{
			val = true;
		}
		else if(str == "\xa1\xb6")
		{
			val = true;
		}
		else if(str == "\xa1\xb7")
		{
			val = true;
		}
		else if(str == "\xa1\xb4")
		{
			val = true;
		}
		else if(str == "\xa1\xb5")
		{
			val = true;
		}
		else if(str == "\xa1\xbe")
		{
			val = true;
		}
		else if(str == "\xa1\xbf")
		{
			val = true;
		}
		return val;
	}
	/*  
    *************************************************
    功能   :判断字符串是否为中文字符
    参数   :
    返回值 :
    -------------------------------------------------
    备注   :不包括汉语标点符号，并且好像只是gbk，不是扩充集。
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-28
    *************************************************
	*/
	bool StringType::IsCHBGKChar(const std::string& str)
	{
		if (str.size() != 2) 
		{
			return false;
		}
		int index = ((unsigned char)str[0]-176)*94 + (unsigned char)str[1] - 161;
		if (index >= 0 && index < 6768) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int StringType::CHGBKCharIndex(const std::string &chChar)
	{
		if((chChar.size() == 2))
		{
			return ((unsigned char)chChar[0]-176)*94 + (unsigned char)chChar[1] - 161;
		}
		else
		{
			return 0;
		}
		
	}
}