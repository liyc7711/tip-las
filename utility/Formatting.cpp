#include "Formatting.h"

namespace utility_train
{
	Formatting::Formatting()
	{
		tokenize  = new Tokenize();
		delimiter = " ";
		head      = "<#>";
		tail      = "</#>";
	}

	Formatting::~Formatting()
	{
		delete tokenize;
	}

	void  Formatting::WsTrain(const std::string& text, std::vector<std::vector<std::string> >& features, const std::string& language, const std::string& encode)
	{
		features.clear();
		std::string segs[] = { " ", "/" };
		std::vector<std::string> words;
		StringSplit::SplitByTokens(words, text, segs, 2, false);
		int size = words.size();
		for (int i = 0; i < size; i++)
		{
			std::vector<std::pair<std::string, std::string> >tmp;
			WSTag(words[i], tmp, language, encode);
			for (int j = 0; j < tmp.size(); j++)
			{
				std::vector<std::string> f;
				f.push_back(tmp[j].first);
				f.push_back(tmp[j].second);
				features.push_back(f);
			}
		}
	}
	/*
	*************************************************
	功能   : 由测试语料生成训练需要的格式
	参数   :
	返回值 :
	-------------------------------------------------
	备注   :
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2013-3-13,2015-10-24
	*************************************************
	*/
	void Formatting::WsTest(const std::string& text, std::vector<std::vector<std::string> >& features, std::vector<bool> & seg,
		                    const std::string& language, const std::string& encode)
	{
		if (text.empty())
		{
			return;
		}
		features.clear();
		seg.clear();
		std::vector< std::string> tokens;
		if (language == "TB")
		{
			Tokenize::TibetanAll(text, tokens);
		}
		for (int i = 0; i< tokens.size(); i++)
		{
			bool s = false;
			if (StringOperation::IsPostfix(tokens[i], "\xe0\xbc\x8b"))
			{
				if (tokens[i].size() > 3)
				{
					tokens[i]= tokens[i].substr(0, tokens[i].size() - 3);
					s = true;
				}
			}
			seg.push_back(s);
			std::vector<std::string> f;
			f.push_back(tokens[i]);
			features.push_back(f);
		}
	}
	void  Formatting::PosTrainS(const std::string& text, std::vector<std::vector<std::string> >& features)
	{
		features.clear();
		std::vector<std::pair<std::string, std::string> > tokens;
		std::string tmp = text;
		POSExtract::ParsePOS(tmp, tokens);
		std::vector<std::string>sc;//current
		std::vector<std::string>sl;//left
		std::vector<std::string>sr;//right
		for (int i = 0; i<tokens.size(); i++)
		{
			if (StringOperation::IsPostfix(tokens[i].first, "\xe0\xbc\x8b"))
			{
				tokens[i].first = tokens[i].first.substr(0, tokens[i].first.size() - 3);
			}
		}
		for (int i = 0; i < tokens.size(); i++)
		{
			std::vector<std::string> f;
			f.push_back(tokens[i].first);
			/******syllables features********************/
			Tokenize::TibetanAll(tokens[i].first, sc);
			if (i > 0)
			{
				Tokenize::TibetanAll(tokens[i - 1].first, sl);
			}
			if (i < (tokens.size() - 1))
			{
				Tokenize::TibetanAll(tokens[i + 1].first, sr);
			}
			/*current syllables*/
			if (sc.size() > 2)
			{
				f.push_back(sc[0]);
				f.push_back(sc[1]);
			}
			else if (sc.size() == 1)
			{
				f.push_back(sc[0]);
				f.push_back(sc[0]);
			}
			else
			{
				f.push_back(head);
				f.push_back(tail);
			}
			/*left syllables*/
			if (sl.size() > 1)
			{
				f.push_back(sl[ sl.size() - 1]);
			}
			else
			{
				f.push_back( tail );
			}
			/*right syllables*/
			if (sr.size() > 1)
			{
				f.push_back(sr[sr.size() - 1]);
			}
			else
			{
				f.push_back(head);
			}
			/******syllables features********************/
			f.push_back(tokens[i].second);   // 
			//f.push_back(tokens[i].second.substr(0, 1)); //
			features.push_back(f);
		}
	}

	void Formatting::PosTestS(const std::string& text, std::vector<std::vector<std::string> >& features, std::vector<bool>& seg)
	{
		features.clear();
		std::string segs[] = { " ", "/" };
		std::vector<std::string> words;
		std::vector<std::string>sc; //current
		std::vector<std::string>sl; //left
		std::vector<std::string>sr ;//right
		StringSplit::SplitByTokens(words, text, segs, 2, false);
		for (int i = 0; i < words.size(); i++)
		{
			bool s = false;
			if (StringOperation::IsPostfix(words[i], "\xe0\xbc\x8b"))
			{
				if (words[i].size() > 3)
				{
					words[i] = words[i].substr(0, words[i].size() - 3);
					s = true;
				}
			}
			seg.push_back(s);
			std::vector<std::string> f;
			f.push_back(words[i]);
			/******syllables features********************/
			Tokenize::TibetanAll(words[i], sc);
			if (i > 0)
			{
				Tokenize::TibetanAll(words[i - 1], sl);
			}
			if (i < (words.size() - 1))
			{
				Tokenize::TibetanAll(words[i + 1], sr);
			}
			/*current syllables*/
			if (sc.size() > 2)
			{
				f.push_back(sc[0]);
				f.push_back(sc[1]);
			}
			else if (sc.size() == 1)
			{
				f.push_back(sc[0]);
				f.push_back(sc[0]);
			}
			else
			{
				f.push_back(head);
				f.push_back(tail);
			}
			/*left syllables*/
			if (sl.size() > 1)
			{
				f.push_back(sl[sl.size() - 1]);
			}
			else
			{
				f.push_back(tail);
			}
			/*right syllables*/
			if (sr.size() > 1)
			{
				f.push_back(sr[sr.size() - 1]);
			}
			else
			{
				f.push_back(head);
			}
			/******syllables features********************/
			features.push_back(f); //add
		}
	}
	void  Formatting::PosTrain(const std::string& text, std::vector<std::vector<std::string> >& features)
	{
		features.clear();
		std::vector<std::pair<std::string, std::string> > tokens;
		std::string tmp = text;
		POSExtract::ParsePOS(tmp,tokens);
		for (int i = 0; i<tokens.size(); i++)
		{
			if (StringOperation::IsPostfix(tokens[i].first, "\xe0\xbc\x8b"))
			{
				tokens[i].first = tokens[i].first.substr(0, tokens[i].first.size() - 3);
			}
		}
		for (int i = 0; i < tokens.size(); i++)
		{
			std::vector<std::string> f;
			f.push_back(tokens[i].first);   // 
			//f.push_back(tokens[i].second.substr(0, 1));
			features.push_back(f);
		}
	}

	void Formatting::PosTest(const std::string& text, std::vector<std::vector<std::string> >& features, std::vector<bool>& seg)
	{
		features.clear();
		std::string segs[] = { " ", "/" };
		std::vector<std::string> words;
		StringSplit::SplitByTokens(words, text, segs, 2, false);
		for (int i = 0; i < words.size(); i++)
		{
			bool s = false;
			if (StringOperation::IsPostfix(words[i], "\xe0\xbc\x8b"))
			{
				if (words[i].size() > 3)
				{
					words[i] = words[i].substr(0, words[i].size() - 3);
					s = true;
				}
			}
			seg.push_back(s);
			std::vector<std::string> f;
			f.push_back(words[i]);
			features.push_back(f);
		}
	}
	
	/*
	*************************************************
	功能   : 对字进行位置标注
	参数   :
	返回值 :
	-------------------------------------------------
	备注   :
	-------------------------------------------------
	作者   ：Li Yachao
	时间   ：2013-3-13, 2015-10-26
	*************************************************
	*/
	void Formatting::WSTag(const std::string &word, std::vector<std::pair<std::string, std::string> > &tags, const std::string &language,const std::string& code)
	{
		tags.clear();
		std::vector<std::string>tokens;
		if (language == "TB")
		{
			Tokenize::TibetanAll(word, tokens);
			for (int i = 0; i < tokens.size(); i++)
			{
				if (StringOperation::IsPostfix(tokens[i], "\xe0\xbc\x8b"))
				{
					if (tokens[i].size() > 3)
					{
						tokens[i] = tokens[i].substr(0, tokens[i].size() - 3);
					}
				}
			}
		}
		else if (language == "CHS")
		{
			if (code == "ANSI")
			{
				Tokenize::Chinese_ANSI(word, tokens);
			}
			else
			{
				Tokenize::Chinese_UTF8(word, tokens);
			}
		}
		
		else
		{
			return;
		}
		int size = tokens.size();
		if (size == 1)
		{
			std::pair<std::string, std::string>p;
			p.first = tokens[0];
			p.second = "S";
			tags.push_back(p);
		}
		else if (size == 2)
		{
			std::pair<std::string, std::string>p;
			p.first = tokens[0];
			p.second = "B";
			tags.push_back(p);
			p.first = tokens[1];
			p.second = "E";
			tags.push_back(p);
		}
		else if (size >= 3)
		{
			for (int i = 0; i<size; i++)
			{
				std::pair<std::string, std::string>p;
				p.first = tokens[i];
				if (i == 0)
				{
					p.second = "B";
				}
				else if (i == size - 1)
				{
					p.second = "E";
				}
				else
				{
					p.second = "M";
				}
				tags.push_back(p);
			}
		}
		else
		{
			return;
		}
	}
	
}
