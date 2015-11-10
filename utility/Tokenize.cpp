#include "Tokenize.h"

namespace utility
{
	Tokenize::Tokenize()
	{
		//str_type = new StringType();
	}
	Tokenize::~Tokenize()
	{
		//delete str_type;
	}
	/*
	*************************************************
	功能   : 中文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : CH表示汉字，PU表示符号，EN表示英文字符，本方法把连续的英文字符组合
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-10
	*************************************************
	*/
	void Tokenize::GBK(const std::string &line, std::vector<std::pair<std::string,std::string> > & val)
	{
		val.clear();
		std::string buf="";
		for(int i=0;i<line.size();i++)
		{
			std::string myChar = line.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			if (myCh1 < 128)
			{
				if((unsigned short)myCh1 == 32)
				{
					if(!buf.empty())
					{
						std::pair<std::string,std::string>p;
						p.first = buf;
						p.second = "EN";
						//vec_val.push_back(buf);
						val.push_back(p);
						buf.clear();
					}
				}
				else
				{
					buf.append(myChar);
				}
			}
			else
			{
				if(!buf.empty())
				{
					std::pair<std::string,std::string>p;
					p.first = buf;
					p.second = "EN";
					//vec_val.push_back(buf);
					val.push_back(p);
					buf.clear();
				}
				myChar+=line.at(i+1);
				i += 1;
				std::pair<std::string,std::string>p;
				p.first = myChar;
				int t = str_type->CH_GBKCharType(myChar);
				if((t == 1))
				{
					p.second = "CH";
				}
				else if((t == 2) || (t == 3))
				{
					p.second = "PU";
				}
				else
				{
					p.second = "EN";
				}
				val.push_back(p);
			}
		}
		if(!buf.empty())
		{
			std::pair<std::string,std::string>p;
			p.first = buf;
			p.second = "EN";
			val.push_back(p);
		}
	}
	/*
	*************************************************
	功能   : 中文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : CH表示汉字，PU表示符号，EN表示英文字符，把英文字符切分开
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-10
	*************************************************
	*/
	void Tokenize::GBKAll(const std::string & line, std::vector<std::pair<std::string,std::string> > & val)
	{
		val.clear();
		for(int i=0;i<line.size();i++)
		{
			std::string myChar = line.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			std::pair<std::string,std::string>p;
			if (myCh1 < 128)
			{
				p.second = "EN";
			}
			else
			{
				myChar+=line.at(i+1);
				i += 1;
				int t = str_type->CH_GBKCharType(myChar);
				if((t == 1))
				{
					p.second = "CH";
				}
				else if((t == 2) || (t == 3))
				{
					p.second = "PU";
				}
				else
				{
					p.second = "EN";
				}
			}
			p.first = myChar;
			val.push_back(p);
		}
	}
	/*
	*************************************************
	功能   : 中文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : UTF8编码方式，对于中文的可以切分出中文和英文的各个字符
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-5
	*************************************************
	*/
	void Tokenize::Chinese_UTF8(const std::string& line, std::vector<std::string>& vec_val)
	{
		vec_val.clear();
		if(line.empty())
		{
			return ;
		}
		StringSplit::SplitUTF8(line, vec_val);
	}
	/*
	*************************************************
	功能   : 中文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : ANSI编码方式，对连续的英文单独切分
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-5
	*************************************************
	*/
	void Tokenize::Chinese_ANSI(const std::string& line, std::vector<std::string>& vec_val)
	{
		vec_val.clear();
		std::string buf="";
		for(int i=0;i<line.size();i++)
		{
			std::string myChar = line.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			if (myCh1 < 128)
			{
				buf.append(myChar);
			}
			else
			{
				if(!buf.empty())
				{
					vec_val.push_back(buf);
					buf.clear();
				}
				myChar+=line.at(i+1);
				i += 1;
				vec_val.push_back(myChar);
			}
		}
		if(!buf.empty())
		{
			vec_val.push_back(buf);
		}
	}
	void Tokenize::Chinese_ANSI(const std::string& line, std::list<std::string>& vec_val)
	{
		vec_val.clear();
		std::string buf="";
		for(int i=0;i<line.size();i++)
		{
			std::string myChar = line.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			if (myCh1 < 128)
			{
				buf.append(myChar);
			}
			else
			{
				if(!buf.empty())
				{
					vec_val.push_back(buf);
					buf.clear();
				}
				myChar+=line.at(i+1);
				i += 1;
				vec_val.push_back(myChar);
			}
		}
		if(!buf.empty())
		{
			vec_val.push_back(buf);
		}
	}
	void Tokenize::Chinese_ANSIALL(const std::string &line, std::vector<std::string> &vec_val)
	{
		vec_val.clear();
		for(int i=0;i<line.size();i++)
		{
			std::string myChar = line.substr(i, 1);
			unsigned char myCh1 = (unsigned char)myChar.at(0);
			if (myCh1 < 128)
			{
			}
			else
			{
				myChar+=line.at(i+1);
				i += 1;
				//vec_val.push_back(myChar);
			}
			vec_val.push_back(myChar);
		}
	}
	/*
	*************************************************
	功能   : 藏文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : UTF8编码方式，对于藏文是纯藏文的文本输入，对于音节缩减的问题不予处理
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-5
	*************************************************
	*/
	void Tokenize::Tibetan(const std::string & line,std::list<std::string>& val)
	{
		std::vector<std::string>vec_val;
		Tibetan(line,vec_val);
		val.clear();
		val.assign(vec_val.begin(),vec_val.end());
	}
	/*
	*************************************************
	功能   : 藏文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : UTF8编码方式，对于藏文是纯藏文的文本输入，对于音节缩减的问题不予处理，
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-5,2015-11-6
	*************************************************
	*/
	void Tokenize::Tibetan(const std::string & line,std::vector<std::string>& val)
	{
		val.clear();
		std::string tokens[]={" ","/","\x09"};
		std::string tokens_u[]={"\xe0\xbc\x8b"};
		std::string tokens_u1= "\xe0\xbc\x8d";
		//std::string tokens_u1= "///";
		std::vector<std::pair<std::string,std::string> > vec_classfication;
		std::vector<std::string> vec_units;
		std::vector<std::string> vec_tmp;
		std::vector<std::string> tmp;
		std::pair<std::string,std::string> kv;
		kv.first = line;
		kv.second ="4";
		TextClassification::TB_UTF8(line,vec_classfication);
		for(int i=0;i<vec_classfication.size();i++)
		{
			if(vec_classfication[i].second == "4")
			{/*处理文本中不同的类别问题，分出英文和藏文字符等*/
				StringSplit::SplitByTokens(vec_tmp,vec_classfication[i].first,tokens_u,1,true);
				/*按照音节点进行切分*/
				for(int j=0;j<vec_tmp.size();j++)
				{/*处理单垂符和前一个字符连着的情况*/
					StringSplit::SplitToken(tmp,vec_tmp[j],tokens_u1);
					for(int k=0;k<tmp.size();k++)
					{
						vec_units.push_back(tmp[k]);
					}
					tmp.clear();
				}
				vec_tmp.clear();
			}
			else
			{
				val.push_back(vec_classfication[i].first);
				//TextProcess::SplitUTF8(vec_classfication[i].key,vec_units);
			}
			for(int j=0;j<vec_units.size();j++)
			{
				val.push_back(vec_units[j]);
			}
			vec_units.clear();
		}
		vec_classfication.clear();
	}
	void Tokenize::Tibetan(const std::string & line,std::vector<std::pair<std::string,std::string> > & val)
	{
		val.clear();
		std::string tokens[]={" ","/","\x09"};
		std::string tokens_u[]={"\xe0\xbc\x8b"};
		std::string tokens_u1= "\xe0\xbc\x8d";
		std::vector<std::pair<std::string,std::string> > vec_classfication;
		std::vector<std::string> vec_units;
		std::vector<std::string> vec_tmp;
		std::vector<std::string> tmp;
		std::pair<std::string,std::string> kv;
		kv.first = line;
		kv.second ="4";
		TextClassification::TB_UTF8(line,vec_classfication);
		for(int i=0;i<vec_classfication.size();i++)
		{
			if(vec_classfication[i].second == "4")
			{/*处理文本中不同的类别问题，分出英文和藏文字符等*/
				StringSplit::SplitByTokens(vec_tmp,vec_classfication[i].first,tokens_u,1,true);
				/*按照音节点进行切分*/
				for(int j=0;j<vec_tmp.size();j++)
				{/*处理单垂符和前一个字符连着的情况*/
					StringSplit::SplitToken(tmp,vec_tmp[j],tokens_u1);
					for(int k=0;k<tmp.size();k++)
					{
						vec_units.push_back(tmp[k]);
					}
					tmp.clear();
				}
				vec_tmp.clear();
			}
			else
			{
				if((vec_classfication[i].second == "0") || (vec_classfication[i].second == "1") || (vec_classfication[i].second == "2"))
				{
					vec_classfication[i].second =  "EN";
				}
				else if (vec_classfication[i].second == "3")
				{
					vec_classfication[i].second =  "PU";
				}
				val.push_back(vec_classfication[i]);
			}
			for(int j=0;j<vec_units.size();j++)
			{
				std::pair<std::string,std::string>p;
				p.first = vec_units[j];
				if(vec_units[j] == tokens_u1)
				{
					p.second = "TPU";
				}
				else
				{
					p.second = "TB";
				}
				val.push_back(p);
			}
			vec_units.clear();
		}
		vec_classfication.clear();
	}
	/*
	*************************************************
	功能   : 藏文文本预处理，序列化输入
	参数   :
	返回值 :
	-------------------------------------------------
	备注   : UTF8编码方式，对于藏文是纯藏文的文本输入，对于音节缩减的问题予以全部切分
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2012-3-5,2015-11-6
	*************************************************
	*/
	void Tokenize::TibetanAll(const std::string & line,std::list<std::string>& val)
	{
		std::vector<std::string>vec_val;
		TibetanAll(line,vec_val);
		val.clear();
		val.assign(vec_val.begin(),vec_val.end());

	}
	void Tokenize::TibetanAll(const std::string & line,std::vector<std::string>& val)
	{
		std::string syllables[6];
		            syllables[0] = "\xe0\xbd\xa6\xe0\xbc\x8b";
					syllables[1] = "\xe0\xbd\xa2\xe0\xbc\x8b";
					syllables[2] = "\xe0\xbd\xa0\xe0\xbd\xb2\xe0\xbc\x8b";
					syllables[3] = "\xe0\xbd\xa0\xe0\xbd\xbc\xe0\xbc\x8b";
					syllables[4] = "\xe0\xbd\xa0\xe0\xbd\x84\xe0\xbc\x8b";
					syllables[5] = "\xe0\xbd\xa0\xe0\xbd\x98\xe0\xbc\x8b";
		std::vector<std::string> tmp;
		Tibetan(line,tmp);
		val.clear();
		for(int i=0;i<tmp.size();i++)
		{
			bool add = false;
			for(int j=0;j<6;j++)
			{
				if(syllables[j] == tmp[i])
				{
					add = true;
					break;
				}
			}
			if(add)
			{
				val.push_back(tmp[i]);
			}
			else
			{
				int index = -1;
				bool sub = false;
				for(int k=0;k<6;k++)
				{
					if(StringOperation::IsPostfix(tmp[i],syllables[k]))
					{
						index  = k;
						break;
					}
				}
				if((index >=0) && (index <=5))
				{
					int length = tmp[i].size() - syllables[index].size();
					std::string sub = tmp[i].substr(0,length);
					if(!sub.empty())
					{
						val.push_back(sub);
					}
					val.push_back(syllables[index]);
				}
				else
				{
					val.push_back(tmp[i]);
				}
			}
		}
	}
	void Tokenize::TibetanAll(const std::string & line,std::vector<std::pair<std::string,std::string> > & val)
	{
		std::string syllables[6];
					syllables[0] = "\xe0\xbd\xa6\xe0\xbc\x8b";
					syllables[1] = "\xe0\xbd\xa2\xe0\xbc\x8b";
					syllables[2] = "\xe0\xbd\xa0\xe0\xbd\xb2\xe0\xbc\x8b";
					syllables[3] = "\xe0\xbd\xa0\xe0\xbd\xbc\xe0\xbc\x8b";
					syllables[4] = "\xe0\xbd\xa0\xe0\xbd\x84\xe0\xbc\x8b";
					syllables[5] = "\xe0\xbd\xa0\xe0\xbd\x98\xe0\xbc\x8b";
		std::vector<std::pair<std::string,std::string> > tmp;
		Tibetan(line,tmp);
		val.clear();
		for(int i=0;i<tmp.size();i++)
		{
			if(tmp[i].second != "TB")
			{
				val.push_back(tmp[i]);
				continue;
			}
			bool add = false;
			for(int j=0;j<6;j++)
			{
				if(syllables[j] == tmp[i].first)
				{
					add = true;
					break;
				}
			}
			if(add)
			{
				std::pair<std::string,std::string>p;
				p.first = tmp[i].first;
				p.second = "TB";
				val.push_back(p);
			}
			else
			{
				int index = -1;
				bool sub = false;
				for(int k=0;k<6;k++)
				{
					if(StringOperation::IsPostfix(tmp[i].first,syllables[k]))
					{
						index  = k;
						break;
					}
				}
				if((index >=0) && (index <=5))
				{
					int length = tmp[i].first.size() - syllables[index].size();
					std::string sub = tmp[i].first.substr(0,length);
					if(!sub.empty())
					{
						std::pair<std::string,std::string>p;
						p.first = sub;
						p.second = "TB";
						val.push_back(p);
					}
					std::pair<std::string,std::string>p;
					p.first = syllables[index];
					p.second = "TB";
					val.push_back(p);
				}
				else
				{
					std::pair<std::string,std::string>p;
					p.first = tmp[i].first;
					p.second = "TB";
					val.push_back(p);
				}
			}
		}
	}
}
