#include "StringOperation.h"

namespace utility
{
	bool StringOperation::IsPrefix(const std::string& src, const std::string& prefix)
	{
		size_t s1=prefix.size();
		size_t s2=src.size();
		if(s1 == 0)
		{
			return true ;
		}
		if( s1>s2 ) 
		{
			return false;
		}
		size_t i=0;
		while( i<s1 && src[i]==prefix[i])
		{
			++i;
		}
		return (i == s1);
	}
	bool StringOperation::IsPostfix(const std::string& src, const std::string& postfix)
	{
		size_t s1=postfix.size();
		size_t s2=src.size();
		if( s1>s2 )
		{
			return false;
		}
		int i=(int)postfix.size()-1;
		int j=(int)src.size()-1;
		while( i>=0 && src[j]==postfix[i])
		{
			--i, --j;
		}
		return (i == -1);
	}
	/*  
    *************************************************
    功能   :剔除字符串中所有的空格、制表符、换行符
    参数   : 
    返回值 :
    -------------------------------------------------
    备注   :覆盖原来的字符串
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	void StringOperation::TrimAll(std::string& str)
	{
		str.erase(0,str.find_first_not_of(" \t\r\n"));
		str.erase(str.find_last_not_of(" \t\r\n")+1); 
	}
	/*  
    *************************************************
    功能   :剔除字符串中首尾的空格
    参数   : 
    返回值 :
    -------------------------------------------------
    备注   :覆盖原来的字符串
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	void StringOperation::TrimBlanks(std::string& str)
	{
		if(str.size() == 0)
		{
			return ;
		}
		size_t i=0, j=0, size=0;
		size = str.size();
		while( i<size && str[i]==' ' )
		{
			++i;
		}
		j=size-1;
		while( j>0 && str[j]==' ' )
		{
			--j;
		}
	    str = str.substr(i, j-i+1);
		
	}
	/*  
    *************************************************
    功能   :剔除字符串中首部的空格
    参数   : 
    返回值 :
    -------------------------------------------------
    备注   :覆盖原来的字符串
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	void StringOperation::TrimBlanksBegin(std::string& str)
	{
		if(str.size() == 0)
		{
			return ;
		}
		size_t size=0;
		size = str.size();
		std::string::size_type i=0;
		while( i<size && str[i]==' ' )
		{
			++i;
		}
		str = str.substr(i);
	}
	/*  
    *************************************************
    功能   :剔除字符串中尾部的空格
    参数   : 
    返回值 :
    -------------------------------------------------
    备注   :覆盖原来的字符串
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	void StringOperation::TrimBlanksEnd(std::string& str)
	{
		if(str.size() == 0)
		{
			return ;
		}
		size_t i=0, j=0, size=0;
		size = str.size();
		j=size-1;
		while( j>0 && str[j]==' ' )
		{
			--j;
		}
		str = str.substr(i, j-i+1);
	}
	/*  
    *************************************************
    功能   :一个字符串是否包含另外一个字符串
    参数   : src源字符串，obj要匹配的目标出现字符串
    返回值 :
    -------------------------------------------------
    备注   :两个字符串不能为空，不然返回false
    -------------------------------------------------
    作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	bool StringOperation::Contains(const std::string src, const std::string obj)
	{
		bool val = false;
		if(src.empty() || obj.empty())
		{
			return val;
		}
		int index = src.find(obj,0);
		if(index >= 0)
		{
			val = true;
		}
		return val;
	}
	/*  
    *************************************************
    功能   :一个字符串在另外一个字符串出现的次数
    参数   : src源字符串，obj要匹配的目标出现字符串
    返回值 :出现的次数
    -------------------------------------------------
    备注   :两个字符串不能为空，不然返回0
    -------------------------------------------------
	作者   ：Li Yachao
    时间   ：2011-11-30
    *************************************************
	*/
	int StringOperation::MatchingNumber(const std::string& src, const std::string& obj)
	{
		int val = 0;
		if(src.empty() || obj.empty())
		{
			return val;
		}
		int length = src.length();
		int start = 0;
		int offset = 0;
		while(start < length)
		{
			offset = src.find( obj , start);
			if(offset >=0)
			{
				val ++;
				start = offset + obj.length();
			}
			else
			{/*注意，没有这个，可能出错*/
				break;
			}
		}
		return val;
	}
	/*  
    *************************************************
    功能   :把字符串按照一定的标识符切割成字符串对
    参数   : 
    返回值 :
    -------------------------------------------------
    备注   :
    -------------------------------------------------
	作者   ：Li Yachao
    时间   ：2011-12-2
    *************************************************
	*/
	void StringOperation::String2Pair(const std::string& str,std::pair<std::string,std::string>& kv, const std::string& seg)
	{
		std::string::size_type pos;
		pos = str.find_first_of(seg, 0);
		kv.first = str.substr(0,pos);
		if (pos != std::string::npos) 
		{
			kv.second = str.substr(pos + seg.size() , str.size() - pos - seg.size());
		}
		else
		{
			kv.second =  "";
		}
	}
	void StringOperation::Replace(std::string& text, const std::string& old_str, const std::string& new_str)
	{
		if(old_str.empty())
		{
			return ;
		}
		std::string::size_type pos=0;
		std::string::size_type srclen=old_str.size();
		std::string::size_type dstlen=new_str.size();
		while( (pos=text.find(old_str, pos)) != std::string::npos)
		{
			text.replace(pos, srclen, new_str);
			pos += dstlen;
		}
	}
	void StringOperation::Remove(std::string& text, const std::string& remove_str)
	{
		Replace(text,remove_str,"");
	}
	void StringOperation::Remove(std::string& text, const std::string remove_strs[], int remove_count)
	{
		for(int i=0;i<remove_count;i++)
		{
			if(remove_strs[i].empty())
			{
				continue ;
			}
			Replace(text,remove_strs[i],"");
		}
	}
}