#include "SentenceSplit.h"

namespace utility
{
	/*
	*************************************************
	功能   : 切分藏文句子
	参数   :
	返回值 :
	-------------------------------------------------
	备注   :
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-18
	*************************************************
	*/
	bool SentenceSplit::Tibetan(const std::string& line, std::vector<std::pair<std::string,std::string> >& val_sentence)
	{
		val_sentence.clear();
		if(line.empty())
		{
			return false;
		}
		std::string seg_flags[]={" ","\xe0\xbc\x8d"};
		//std::string seg_flags[]={"4","3"};
		std::vector<std::pair<std::string,std::string> > val;
		SplitByTokens(val,line,seg_flags,2);
		val_sentence = val;
		val.clear();
		return true;
	}
	bool SentenceSplit::ChineseUTF8(const std::string& line,std::vector<std::pair<std::string,std::string> >& val_sentences)
	{
		val_sentences.clear();
		if(line.empty())
		{
			return false;
		}
		/*
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
		std::vector<std::pair<std::string,std::string> > val;
		SplitByTokens(val,line,chCharList,length);
		val_sentences = val;
		val.clear();
		return true;
	}
	bool SentenceSplit::ChineseANSI(const std::string& line,std::vector<std::pair<std::string,std::string> >& val_sentences)
	{
		val_sentences.clear();
		if(line.empty())
		{
			return false;
		}
		/*
		std::string str="。，、；：
		？！“”‘’
		╗╚（）…—
		《》〈〉•";
		*/
		const int length = 22;
		std::string chCharList[length] = {"。","，","、","；","：",\
			"？","！","“","”","‘","’",\
			"╗","╚","（","）","…","—",\
			"《","》","〈","〉","•"};
		std::vector<std::pair<std::string,std::string> >val;
		SplitByTokens(val,line,chCharList,length);
		val_sentences = val;
		val.clear();
		return true;
	}
	bool SentenceSplit::SplitByTokens(std::vector<std::pair<std::string,std::string> >& vecstr, const std::string& str, const std::string tokens[], const int tokensnumber)
	{
		vecstr.clear();
		if((str.empty()) ||tokensnumber <=0 )
		{
			return  false;
		}
		std::string buffer="";
		int textLength = str.length();
		int start = 0;
		int offset = 0;
		while(start < textLength)
		{
			offset = textLength;
			int subLength =0;
			//std::string tmp ="";
			std::pair<std::string,std::string> kv;kv.first = "";kv.second ="";
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
				kv.first  = str.substr(start,0);
				kv.second =str.substr(offset,subLength);
				start = offset + subLength;
			}
			else if(start < offset)
			{
				kv.first = str.substr(start,offset - start) ;
				kv.second =str.substr(offset,subLength) ;
				start = (offset + subLength  );
			}
			/*这个影响多个空格连在一块，并且切分标志位空格的情况*/
			vecstr.push_back(kv);
		}
		return true;

	}
}

