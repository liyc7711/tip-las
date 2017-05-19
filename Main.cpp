/********************************************************************
* Copyright (C) 2011 - 2015
* The Inistitute of Chinese Ethnic Information Technology ( www.nlit.edu.cn)
* Contact: Li Yachao, harry_lyc@foxmail.com or liyc7711@gmail.com
*
* Permission to use, copy, modify, and distribute this software for
* any non-commercial purpose is hereby granted without fee, provided
* that the above copyright notice appear in all copies and that both
* that copyright notice.
* It is provided "as is" without express or implied warranty.
*
* Version: 1.6
* Last update: 2011-11-28,2014-06-26,2015-02-03,2015-10-23
*********************************************************************/
/********************************************************************
* Thanks to Wang Kun and Li Xiaoqing for the help of this project.
*
*
********************************************************************/

#include "Las.h"

using namespace std;
using namespace Tip;
void  Usage();
void  Copyrgiht();

int main(int argc, char * argv[])
{
	if (argc < 5)
	{
		Usage();
		return 0;
	}
	Copyrgiht();
	std::string task      =  "ws";
	std::string input     =  "intput.txt";
	std::string output    =  "output.txt";
	std::string model_ws  =  "ws.model";
	std::string model_pos =  "pos.model";
	std::string type      =  "ws";
	int iter              =  20;
	int beamSize          =  8;
    int nGram             =  2;
	type   = argv[1];
    task   = argv[2];
	input  = argv[3];
	output = argv[4];
	class Las* las = new Las();
	if (type == "train")
	{
		if (argc >= 6)
		{
			iter = stoi(argv[5]);
		}
		if (task == "ws")
		{
			las->Train(input, output, task, iter,beamSize,nGram);
		}
		else if (task == "pos")
		{// for pos  60 iter,give a better performance
			las->Train(input, output, task, iter*3, beamSize * 2, nGram+1);
		}
		else
		{
			Usage();
		}
	}
	else if (type == "test")
	{
		if (!las->Test(input, output, model_ws, model_pos, task))
		{
			Usage();
		}
	}
        else if (type == "dev")
	{
		if (!las->Development(input))
		{
			Usage();
		}
	}
	else
	{
		Usage();
	}
	delete las;
	return 0;
}
void Usage()
{
	std::cout << "tip-las train  ws/pos      input  model" << std::endl;
	std::cout << "tip-las test   ws/pos/all  input  output" << std::endl;
}
void Copyrgiht()
{
	std::cout << "The Inistitute of Chinese Ethnic Information Technology (www.nlit.edu.cn)" << std::endl;
	std::cout << "Copyright (C) 2011 - 2015." << std::endl;
	std::cout << "Version: 1.6, Last update: 2015-10-23." << std::endl;
	std::cout << "Contact: liyc7711@gmail.com." << std::endl;
}
