#include "markdown.h"
#include <qdebug.h>
#include <qfileinfo.h>

/*
 * @brief: the interface to convert *.md to *.html
 * @param: const string &
 * @return: void
 *          but create a html file
 */
//Adapted from a standard C++ program
void ViewMarkdown(const string &fileName)
{
        QString htmlName = QFileInfo(QString(fileName.c_str())).baseName() + ".html";

        MarkdownToHTMLConverter	converter(fileName);

        converter.wrieHtml(htmlName.toStdString());
}

MarkdownToHTMLConverter::MarkdownToHTMLConverter(std::string inputfilename)
    : final_lines(inputfilename)
{
}

void MarkdownToHTMLConverter::wrieHtml(std::string outputfilename)
{
    std::ofstream outputfile;

    outputfile.open(outputfilename, std::ios::out);
    if(!outputfile.is_open())
	{
		std::cout << "ERROR: HTML FILE COULD NOT BE OPENED" << std::endl;
	}
    outputfile << "<html>\n<body>\n";

	std::string line = "start text";
	while (!line.empty())
	{
        line = final_lines.nextPara();
        outputfile << line;
        outputfile << "\n";
	}
    outputfile << "\n</body>\n</html>";
}

Reader::Reader(std::string inputfilename)
    : reading(true),
      file(inputfilename)
{
    if (!file.is_open())
		throw std::runtime_error(strerror(errno));
	else
        next();
}

std::string Reader::nextLine()
{
    if (reading)
	{
        std::string ret = getcurrent();
        next();
		return ret;
	}
	else
		return std::string();
}

std::string Reader::getcurrent()
{
    return current;
}

void Reader::next()
{
    std::string line = "some text";
    current = "";
    while(reading && line != "")
    {
        if (!std::getline(file, line))
            reading = false;
        else
            current += (line + " ");
    }
}

Tag_line::Tag_line(std::string inputfilename)
        : reader(inputfilename),
          finished(false)
{
    regexps[h1] = "(#)\\s+(.*)";
    regexps[h2] = "(##)\\s+(.*)";
    regexps[h3] = "(###)\\s+(.*)";
    regexps[h4] = "(####)\\s+(.*)";
    regexps[h5] = "(#####)\\s+(.*)";
    regexps[h6] = "(######)\\s+(.*)";
    regexps[ul] = " \\* (.*)";
    regexps[ol] = "(.*)[ X|\tX]*[0-9]+. (.*)";
    next();
}

std::string Tag_line::nextPara()
{
    if (!finished)
	{
        std::string ret = getcurrent();
        next();
		return ret;
	}
	else
		return std::string();
}

std::string Tag_line::getcurrent()
{
    return current;
}
//The following is the kernel part of Mark2Html
void Tag_line::next()
{
    std::string raw_line = reader.nextLine();
	if (raw_line.empty())
        finished = true;
    else
    {
		raw_line = raw_line.substr(0,  raw_line.length() - 2);
        if (std::regex_match(raw_line, std::regex(regexps[h1])))
            current = "<h1>" + raw_line.substr(2, sizeof(raw_line)) + "</h1>";
        else if (std::regex_match(raw_line, std::regex(regexps[h2])))
            current = "<h2>" + raw_line.substr(3, sizeof(raw_line)) + "</h2>";
        else if (std::regex_match(raw_line, std::regex(regexps[h3])))
            current = "<h3>" + raw_line.substr(4, sizeof(raw_line)) + "</h3>";
        else if (std::regex_match(raw_line, std::regex(regexps[h4])))
            current = "<h4>" + raw_line.substr(5, sizeof(raw_line)) + "</h4>";
        else if (std::regex_match(raw_line, std::regex(regexps[h5])))
            current = "<h5>" + raw_line.substr(6, sizeof(raw_line)) + "</h5>";
        else if (std::regex_match(raw_line, std::regex(regexps[h6])))
            current = "<h6>" + raw_line.substr(7, sizeof(raw_line)) + "</h6>";
        else if (std::regex_match(raw_line, std::regex(regexps[ul])))
		{
            current = "<ul>" + raw_line + "</ul>";
            current = unorderedlist(current);
		}
        else
            current = "<p>" + raw_line + "</p>";
        current = orderedList(current);
        current = link(current);
        current = italicAndBold( current);
    }
}

std::string Tag_line::link(std::string str)
{
	auto pos = str.find("[");
	std::string start = str.substr(0, pos);
	std::string finish = str.substr(pos + 1, std::string::npos);
	if (pos != std::string::npos && str[pos - 1] != '!')
	{
		auto end = finish.find("]");
		std::string link_name = str.substr(pos + 1, end);
		std::string link = finish.substr(end + 2, std::string::npos);

		auto link_end = link.find(")");
		link = link.substr(0, link_end);
		link_end = finish.find(")");
		finish = finish.substr(link_end + 1, std::string::npos);
		str = start + "<a href=\"" + link + "\">" + link_name + "</a>" + finish;
	}
	return str;
}

std::string Tag_line::orderedList(std::string str)
{
	auto pos = str.find(". ");
	if (pos != std::string::npos && str[pos - 1] <= '9' && str[pos - 1] >= '0')
	{
		std::string small = str.substr(pos - 2, std::string::npos);
		str = str.substr(0, pos - 1) + "<ol><li>";
		pos = small.find("\n");
		std::string end = small.substr(pos + 3,  std::string::npos);
		small = small.substr(3, pos);
		pos = small.find("</");
		small = small.substr(0, pos) +  "</li></ol>" + small.substr(pos, std::string::npos);
		pos = small.find(". ");
		while ((pos < std::string::npos))
		{
			small = small.substr(0, pos - 2) + "</li><li>" + small.substr(pos + 1, std::string::npos);
			pos = small.find(". ");
		}
		str = str + small + end;
	}
	return str;
}

std::string Tag_line::italicAndBold(std::string str)
{
	unsigned int i = 4;
	bool em_opened = false;
	bool bold_opened = false;
	while (i < str.length() - 3)
	{
		if (bold_opened == false && str[i] == '*' && str[i + 1] == '*' && str[i + 2] != ' ')
		{
			str = str.substr(0, i) + "<strong>" + str.substr(i + 2, str.length());
			bold_opened = true;
			i += 7;
		}
		else if (bold_opened == true && str[i] == '*' && str[i + 1] == '*' && str[i - 1] != ' ')
		{
			str = str.substr(0, i) + "</strong>" + str.substr(i + 2, str.length());
			bold_opened = false;
			i += 2;
		}
		else if (em_opened == false && str[i] == '*' && str[i + 1] != '*' && str[i + 1] != ' ')
		{
			str = str.substr(0, i - 1) + "<em>" + str.substr(i + 1, str.length());
			em_opened = true;
			i += 4;
		}
		else if (em_opened == true && str[i] == '*' && str[i - 1] != ' ')
		{
			str = str.substr(0, i) + "</em>" + str.substr(i + 1, str.length());
			em_opened = false;
			i += 5;
		}
		i++;
	}
	return str;
}

std::string Tag_line::unorderedlist(std::string str)
{
	unsigned int i = 6;
	str = str.substr(0, i - 1) + "\n\t<li>" + str.substr(i, std::string::npos);
	i += 6;
	while (i < str.length() - 6)
	{
		if (str[i] == '*' && str[i + 1] == ' ' && str[i - 1] == ' ')
		{
			str = str.substr(0, i - 1) + "</li>\n\t<li>" + str.substr(i + 1, str.length());
			i += 10;
		}
		i++;
	}
	str = str.substr(0, i + 1) + "</li>\n" + str.substr(i + 1, std::string::npos);
	return str;
}
