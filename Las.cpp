#include "Las.h"

namespace Tip
{
	Las::~Las()
	{
		delete format;
		if (tagging_ws != NULL)
		{
			delete tagging_ws;
		}
		if (tagging_pos != NULL)
		{
			delete tagging_pos;
		}
	}

	Las::Las()
	{
		format       = new  Formatting();
		tagging_ws   = NULL;
		tagging_pos  = NULL;
		templatesSet = "S";
	}

	bool Las::Train(const std::string& trainFile, const std::string& modelFile, const std::string& type, int iter , int beamSize , int nGram)
	{
		std::ifstream fin;
		fin.open(trainFile.c_str());
		if (!fin.is_open())
		{
			std::cerr << "Open [" << trainFile << "] error!" << std::endl;
			return false;
		}
		std::string utf8flag = "\xef\xbb\xbf";
		std::string utf16flag = "\xff\xfe";
		std::string myLine = "";
		int lineIndex = 0;
		std::vector<std::vector<std::string> > fs;
		std::cout << "Loading Trainning Samples..." << std::endl;
		class Model  * model = NULL;
		class Sample * s     = NULL;
		if (type == flag_pos)
		{
			model = new Model(modelFile, iter, templatesSet);
			//model = new Model(modelFile, iter);
		}
		else
		{
			model = new Model(modelFile, iter);
		}
		s = new Sample(model->LeftBound(), model->RightBound());
		while (getline(fin, myLine))
		{
			lineIndex++;
			StringOperation::TrimAll(myLine);
			if (myLine.empty())
			{
				continue;
			}
			if (lineIndex == 1)
			{
				if (StringOperation::IsPrefix(myLine, utf8flag))
				{
					myLine = myLine.substr(3, myLine.length() - 3);
				}
				else if (StringOperation::IsPrefix(myLine, utf16flag))
				{
					std::cout << "File type error,need UTF8 or ANSI file."<<std::endl;
					return false;
				}
				else
				{

				}
			}
			if (type == flag_ws)
			{
				format->WsTrain(myLine, fs);
			}
			else if (type == flag_pos)
			{
				format->PosTrainS(myLine,fs);
			}
			else
			{
				return false;
			}
			if (!s->AddEvents(fs))
			{
				std::cout << "Add Events Error, at Line" << lineIndex << std::endl;
				return false;
			}
			fs.clear();
		}
		fin.close();
		s->AddEventsOver();
		model->LoadSamples(s);
		std::cout << "Start Trainning..." << std::endl;
		clock_t start, end;
		start = clock();
		Perceptron * per = new Perceptron(s, model, true, iter, beamSize, nGram);
		per->Train();
		model->SaveModel(s);
		end = clock();
		std::cout << "done, time cost: " << double(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
		delete s;
		delete model;
		delete per;
		return true;
	}

	bool  Las::Test(const std::string& inFile, const std::string& outFile, const std::string& modelws, const std::string& modelpos, const std::string& type)
	{
		class Sample * sample_ws  = NULL;
		class Sample * sample_pos = NULL;
		class Model  * model_ws   = NULL;
		class Model  * model_pos  = NULL;
		clock_t start, end;
		start = clock();
		if (type == flag_ws)
		{
			model_ws  = new Model();
			sample_ws = new Sample();
			if (!model_ws->ReadModel(modelws, sample_ws))
			{
				std::cout << "Read " << modelws << " Error!!!" << std::endl;
				return false;
			}
			tagging_ws = new Perceptron(sample_ws, model_ws);
		}
		else if (type == flag_pos)
		{
			model_pos  = new Model();
			sample_pos = new Sample();
			if (!model_pos->ReadModel(modelpos, sample_pos))
			{
				std::cout << "Read " << modelpos << " Error!!!" << std::endl;
				return false;
			}
			tagging_pos = new Perceptron(sample_pos, model_pos);
		}
		else if (type == flag_all)
		{
			model_ws  = new Model();
			sample_ws = new Sample();
			if (!model_ws->ReadModel(modelws, sample_ws))
			{
				std::cout << "Read " << modelws << " Error!!!" << std::endl;
				return false;
			}
			tagging_ws = new Perceptron(sample_ws, model_ws);
			sample_pos = new Sample();
			model_pos = new Model();
			if (!model_pos->ReadModel(modelpos, sample_pos))
			{
				std::cout << "Read " << modelpos << " Error!!!" << std::endl;
				return false;
			}
			tagging_pos = new Perceptron(sample_pos, model_pos);
		}
		else
		{
			return false;
		}
		std::ifstream fin;
		std::ofstream fout;
		fin.open(inFile.c_str());
		if (!fin.is_open())
		{
			std::cerr << "Open [" << inFile << "] Error!" << std::endl;
			return false;
		}
		fout.open(outFile.c_str());
		if (!fout.is_open())
		{
			std::cerr << "Open [" << outFile << "] Error!" << std::endl;
			return false;
		}
		std::string utf8flag = "\xef\xbb\xbf";
		std::string utf16flag = "\xff\xfe";
		std::string myLine = "";
		int lineIndex = 0;
		std::vector<std::vector<std::string> > fs;
		std::cout << "Start Tagging..." << std::endl;
		while (getline(fin, myLine))
		{
			lineIndex++;
			if (lineIndex == 1)
			{
				if (StringOperation::IsPrefix(myLine, utf8flag))
				{
					myLine = myLine.substr(3, myLine.length() - 3);
				}
				else if (StringOperation::IsPrefix(myLine, utf16flag))
				{
					std::cout << "File Type Error,need UTF8 or ANSI file.\n";
					return false;
				}
			}
			if (type == flag_ws)
			{
				std::string val = "";
				WsTest(myLine,val);
				fout << val << std::endl;
			}
			else if (type == flag_pos)
			{
				std::string val = "";
				PosTest(myLine, val);
				fout << val << std::endl;
			}
			else if (type == flag_all)
			{
				std::string ws  = "";
				std::string pos = "";
				WsTest(myLine, ws);
				PosTest(ws, pos);
				fout << pos << std::endl;
			}
			else 
			{
				return false;
			}
		}
		fin.close();
		fout.close();
		if (type == flag_ws)
		{
			delete sample_ws;
			delete model_ws;
		}
		else if (type == flag_pos)
		{
			delete sample_pos;
			delete model_pos;
		}
		else if (type == flag_all)
		{
			delete sample_ws;
			delete sample_pos;
			delete model_ws;
			delete model_pos;
		}
		else
		{

		}
		end = clock();
		std::cout << "done, time cost: " << double(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
		return true;
	}

	bool Las::PosTest(const std::string& text, std::string& val)
	{
		val.clear();
		std::vector<std::vector<std::string> > fs;
		std::vector<std::string> result;
		std::vector<bool> segs;
		format->PosTestS(text,fs,segs);
		tagging_pos->Test(fs, result);
		int size = result.size();
		for (int i = 0; i < size; i++)
		{
			val += fs[i][0];
			if (segs[i])
			{
				val += "\xe0\xbc\x8b";
			}
			val += "/";
			val += result[i];
			val += delimiter;
		}
		return true;
	}

	bool Las::WsTest(const std::string& text, std::string& val)
	{
		val.clear();
		std::vector<std::vector<std::string> > fs;
		std::vector<bool> segs;
		std::vector<std::string> result;
		format->WsTest(text, fs, segs);
		tagging_ws->Test(fs, result);
		std::string buffer = "";
		int size = result.size();
		for (int i = 0; i < size; i++)
		{
			buffer += fs[i][0];
			if (segs[i])
			{
				buffer += "\xe0\xbc\x8b";
			}
			if ((result[i] == "E") || (result[i] == "S"))
			{
				val += buffer;
				val += delimiter;
				buffer.clear();
			}
		}
		return true;
	}

	bool Las::WsTest(const  std::string& text, std::vector<std::string>& words, bool withSeg)
	{
		words.clear();
		std::vector<std::vector<std::string> > fs;
		std::vector<bool> segs;
		std::vector<std::string> result;
		format->WsTest(text, fs, segs);
		tagging_ws->Test(fs, result);
		std::string buffer = "";
		int size = result.size();
		for (int i = 0; i < size; i++)
		{
			buffer += fs[i][0];
			if (withSeg && segs[i])
			{
				buffer += "\xe0\xbc\x8b";
			}
			if ((result[i] == "E") || (result[i] == "S"))
			{
				words.push_back(buffer);
				buffer.clear();
			}
		}
		return true;
	}

	bool Las::PosDevelopment(const std::string& text, std::string& val)
	{
		val.clear();
		std::vector<std::vector<std::string> > fs;
		std::vector<std::string> result;
		std::vector<bool> segs;
		format->PosTrainS(text, fs);
		tagging_pos->Test(fs, result);
		int size = result.size();
		if (size == 0)
		{
			return true;
		}
		total += size;
		int index = fs.at(0).size() - 1;
		for (int i = 0; i < size; i++)
		{
			if (result[i] == fs[i][index])
			{
				correct++;
			}
			val += fs[i][0];
			//if (segs[i])
			//{
				//val += "\xe0\xbc\x8b";
			//}
			val += "/";
			val += result[i];
			val += delimiter;
		}
		return true;
	}

	bool Las::Development(const std::string& inFile, const std::string& modelws, const std::string& modelpos, const std::string& type)
	{
		class Sample * sample_ws  = NULL;
		class Sample * sample_pos = NULL;
		class Model * model_ws    = NULL;
		class Model * model_pos   = NULL;
		if (type == flag_ws)
		{
			model_ws  = new Model();
			sample_ws = new Sample();
			if (!model_ws->ReadModel(modelws, sample_ws))
			{
				return false;
			}
			tagging_ws = new Perceptron(sample_ws, model_ws);
		}
		else if (type == flag_pos)
		{
			model_pos  = new Model();
			sample_pos = new Sample();
			if (!model_pos->ReadModel(modelpos, sample_pos))
			{
				return false;
			}
			tagging_pos = new Perceptron(sample_pos, model_pos);
		}
		else
		{
			return false;
		}
		std::ifstream fin;
		fin.open(inFile.c_str());
		if (!fin.is_open())
		{
			std::cerr << "Open [" << inFile << "] Error!" << std::endl;
			return false;
		}
		std::ofstream fout;
		fout.open("devout.txt");
		std::string utf8flag  = "\xef\xbb\xbf";
		std::string utf16flag = "\xff\xfe";
		std::string myLine = "";
		int lineIndex = 0;
		std::vector<std::vector<std::string> > fs;
		std::cout << "Start Tagging..." << std::endl;
		total   = 0;
		correct = 0;
		clock_t start, end;
		start = clock();
		while (getline(fin, myLine))
		{
			lineIndex++;
			if (lineIndex == 1)
			{
				if (StringOperation::IsPrefix(myLine, utf8flag))
				{
					myLine = myLine.substr(3, myLine.length() - 3);
				}
				else if (StringOperation::IsPrefix(myLine, utf16flag))
				{
					std::cout << "File type error,need UTF8 or ANSI file.\n";
					return false;
				}
			}
			if (type == flag_ws)
			{
				std::string val = "";
			}
			else if (type == flag_pos)
			{
				std::string val = "";
				PosDevelopment(myLine, val);
				fout<< val << std::endl;
			}
			else
			{
				return false;
			}
		}
		fin.close();
		if (type == flag_ws)
		{
			delete sample_ws;
			delete model_ws;
		}
		else if (type == flag_pos)
		{
			delete sample_pos;
			delete model_pos;
		}
		else
		{

		}
		end = clock();
		std::cout << "Precision: " << (correct / total) << std::endl;
		std::cout << "done, time cost: " << double(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
		return true;
	}
}
