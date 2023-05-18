// COPYRIGHT 2023, MereGosling
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :

// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#ifndef _ARGUMENT_PARSER_H
#define _ARGUMENT_PARSER_H

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

namespace ArgumentParser
{
	bool PreProcessed = false;

	class Argument
	{
	public:
		std::string Name;
		std::string Value;

		std::vector<std::string> Aliases;

		bool Switch = false;
		bool Present = false;

		void (*Callback)(std::string);

		inline Argument(std::string _Name, std::vector<std::string> _Aliases, bool _Switch, void (*_Callback)(std::string) = NULL, std::string _Value = "")
		{
			Name = _Name;
			Value = _Value;

			Aliases = _Aliases;

			Switch = _Switch;

			Callback = _Callback;
		}

		std::string ToString()
		{
			std::string Output = Name + ":\n\tSwitch:\t\t" + (Switch ? "TRUE" : "FALSE") + "\n\tPresent:\t" + (Present ? "TRUE" : "FALSE") + "\n\tAliases:\t[ ";

			for (int i = 0; i < Aliases.size(); i++)
			{
				Output += "\"" + Aliases[i] + "\"";
				if (i != Aliases.size() - 1)
					Output += ", ";
			}
			Output += " ]";

			if (!Switch)
			{
				Output += "\n\tValue:\t\t" + Value;
			}

			return Output;
		}
	};

	std::vector<Argument> ArgumentList;

	// Process the argument list and populate our list of options.
	void PreProcess(std::vector<Argument> ArgMap, int argc, char** argv)
	{
		PreProcessed = true;

		for (int i = 0; i < ArgMap.size(); i++)
		{
			ArgumentList.push_back(ArgMap[i]);
		}

		std::vector<std::pair<std::string, std::string>> SuppliedArg;

		for (int i = 1; i < argc; i++)
		{
			std::string Val = "";
			std::string arg = argv[i];

			if (arg[0] == '/' || arg[0] == '-')
			{
				if (argc > i + 1)
				{
					Val = argv[i + 1];
				}

				SuppliedArg.push_back(std::make_pair(arg.substr(1, arg.length() -1), Val));
			}
			else
				continue;
		}

		for (int i = 0; i < ArgumentList.size(); i++)
		{
			for (int x = 0; x < SuppliedArg.size(); x++)
			{
				if (SuppliedArg[x].first == ArgumentList[i].Name 
					|| std::count(ArgumentList[i].Aliases.begin(), ArgumentList[i].Aliases.end(), SuppliedArg[x].first))
				{
					ArgumentList[i].Present = true;
					if (!ArgumentList[i].Switch)
						ArgumentList[i].Value = SuppliedArg[x].second;
				
					break;
				}
			}
		}

		for (int i = 0; i < ArgumentList.size(); i++)
		{
			if (!ArgumentList[i].Callback)
				continue;

			if (!ArgumentList[i].Switch)
				ArgumentList[i].Callback(ArgumentList[i].Value);
			else if (ArgumentList[i].Present)
				ArgumentList[i].Callback("");
		}
	}

	// List all our options.
	// Only works if the argument list has been preprocessed.
	void ListArguments()
	{
		printf("--- OPTIONS ---\n");

		if (!PreProcessed)
		{
			printf("The argument list has not been preprocessed!\n");
			return;
		}

		printf("\n");

		for (int i = 0; i < ArgumentList.size(); i++)
		{
			printf(ArgumentList[i].ToString().c_str());
			printf("\n");
		}
	}
}

#endif