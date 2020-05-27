//mark_h
//functions that are used to convert a Markdown file into a temporary HTML file.
//Adapted from a standard C++ program

#ifndef MARK_H
#define MARK_H

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#define REGS 8

enum tagType { h1, h2, h3, h4, h5, h6, ul, ol };

class Reader
{
	public:
        Reader(std::string inputfilename);
        std::string		nextLine();

	private:
        bool 			reading;
        std::ifstream	file;
        std::string		current;

        std::string		getcurrent();
        void			next();

};

class Tag_line
{
	public:
        Tag_line(std::string inputfilename);
        std::string		nextPara();

	private:
        Reader			reader;
        bool			finished;
        std::string		current;
        std::string		regexps[REGS];

        std::string		getcurrent();
        void			next();
        std::string		unorderedlist(std::string str);
        std::string		italicAndBold(std::string str);
        std::string		orderedList(std::string str);
        std::string		link(std::string str);
};

class MarkdownToHTMLConverter
{
	public:
        MarkdownToHTMLConverter(std::string inputfilename);
        void			wrieHtml(std::string outputfilename);

	private:
        Tag_line		final_lines;
};

using namespace std;
void ViewMarkdown(const string &);

#endif
