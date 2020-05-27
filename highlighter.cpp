#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
}

/*
 * @brief: This function is to set the key words.
 * @param: The type of current file, which determines the list of keyword.
 * @return: void.
 */
void Highlighter::setKeyWords(FileType file)
{
    HighlightingRule rule;
    QStringList keywordPatterns;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    highlightingRules.clear();
    // clear the previous highlighting rules whenever the filetype changes.

    switch (file) {
    case CppFile:
    {
        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Black);
        keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                        << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                        << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                        << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                        << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                        << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                        << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                        << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                        << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                        << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
    }
    case CFile:
    case CHeaderFile:
    {
        keywordFormat.setForeground(Qt::darkCyan);
        keywordFormat.setFontWeight(QFont::Black);
        keywordPatterns << "\\basm\\b" << "\\bdo\\b" << "\\bif\\b" << "\\breturn\\b" << "\\btypedef\\b"<<
                           "\\bauto\\b" << "\\bdouble\\b" << "\\binline\\b" << "\\bshort\\b"<<
                           "\\btypeid\\b" << "\\bbool\\b" << "\\bdynamic_cast\\b" << "\\bint\\b" <<
                           "\\bsigned\\b" << "\\btypename\\b" << "\\bbreak\\b" << "\\belse\\b" <<
                           "\\blong\\b" << "\\bsizeof\\b" << "\\bunion\\b" << "\\bcase\\b" << "\\benum\\b" <<
                           "\\bmutable\\b" << "\\bstatic\\b" << "\\bunsigned\\b" << "\\bcatch\\b" <<
                           "\\bexplicit\\b" << "\\bnamespace\\b" << "\\bstatic_cast\\b" << "\\busing\\b"<<
                           "\\bchar\\b" << "\\bexport\\b" << "\\bnew\\b" <<"\\bstruct\\b" << "\\bvirtual\\b" <<
                           "\\bextern\\b" << "\\boperator\\b" << "\\bswitch\\b" << "\\bvoid\\b" <<
                           "\\bcosnt\\b" << "\\bfalse\\b" << "\\bprivate\\b" << "\\btemplate\\b" <<
                           "\\bvolatile\\b" << "\\bconst_cast\\b" << "\\bfloat\\b" << "\\bprotected\\b" <<
                           "\\bthis\\b" << "\\bwchar_t\\b" << "\\bcontinue\\b" << "\\bfor\\b" << "\\bpublic\\b" <<
                           "\\bthrow\\b" << "\\bwhile\\b" << "\\bdefalut\\b" << "\\bfriend\\b" << "\\bregister\\b"<<
                           "\\btrue\\b" << "\\bdelete\\b" << "\\bgoto\\b" << "\\breinterpret_cast\\b" <<"\\btry\\b" <<
                           "\\bcontinue\\b";
        break;
    }

    case HTMLFile:
    case MarkdownFile:{
        keywordPatterns << "\\babsrtract\\b" << "\\benum\\b" << "\\bint\\b" << "\\bshort\\b" << "\\bboolean\\b" <<
                           "\\bexport,interface\\b" << "\\bstatic\\b" << "\\bsuper\\b" << "\\blong\\b" <<
                           "\\bextends\\b" << "\\bbyte\\b" << "\\bchar\\b" << "\\bfinal\\b" << "\\bnative\\b" <<
                           "\\bsynchronized\\b" << "\\bthrows\\b" << "\\bpackage\\b" << "\\bfloat\\b" <<
                           "\\bclass\\b" << "\\bconst\\b" << "\\bgoto\\b" << "\\bprivate\\b" << "\\btransient\\b" <<
                           "\\bvolatile\\b" << "\\bprotected\\b" << "\\bimplements\\b" << "\\bdebugger\\b" <<
                           "\\bdouble\\b" << "\\bimport\\b" << "\\bpublic\\b" << "\\bbreak锛宒o\\b" << "\\binstanceof \\b" <<
                           "\\btypeof\\b" << "\\bcase\\b" << "\\belse\\b" << "\\bnew\\b" << "\\bvar\\b" << "\\bvoid\\b" <<
                           "\\breturn\\b" << "\\bfinally\\b" << "\\bcatch\\b" << "\\bcontinue\\b" << "\\bfor\\b" <<
                           "\\bswitch\\b" << "\\bwhile\\b" << "\\bwith\\b" << "\\bthis\\b" << "\\bfunction\\b" <<
                           "\\bdebugger\\b" << "\\bdefault\\b" << "\\bif\\b" << "\\bthrow\\b" << "\\btry\\b" <<
                           "\\bin\\b" << "\\bdelete\\b"<<
                           "\\bhtml>  <\html>\\b" << "\\bhead>  <\head>\\b" << "\\btitle>  <\title>\\b" <<"\\bstyle>  <\style>\\b" <<
                           "\\bbody>  <\body>\\b" << "\\bcenter>  <\center>\\b" <<"\\btable>  <\table>\\b";
        break;
    }

    case PythonFile:{

        keywordPatterns << "\\band\\b" << "\\belif\\b" << "\\import\\b"
                        << "\\bdouble\\b" << "\\benum\\b" << "\\return\\b"
                        << "\\bas\\b" << "\\blambda\\b" << "\\bint\\b"
                        << "\\bwith\\b" << "\\braise\\b" << "\\bdef\\b"
                        << "\\byield\\b" << "\\bpass\\b" << "\\bor\\b"
                        << "\\bglobal\\b" << "\\bdel\\b" << "\\bexcept\\b"
                        << "\\bnot\\b" << "\\bwhile\\b" << "\\btry\\b"
                        << "\\belse\\b" << "\\bpass\\b" << "\\bfinally\\b"
                        << "\\bassert\\b" << "\\bbreak\\b" << "\\bclass\\b"
                        << "\\bcontinue\\b" << "\\bin\\b" << "\\bis\\b";
        break;


    }

    case JavaFile: {
        keywordPatterns << "\\babstract\\b" << "\\bassert\\b" << "\\bdo\\b" << "\\bif\\b" << "\\breturn\\b" << "\\btypedef\\b"<<
                           "\\bauto\\b" << "\\bdouble\\b" << "\\binline\\b" << "\\bshort\\b"<< "\\bboolean\\b" << "\\bbyte\\b" <<
                           "\\btypeid\\b" << "\\bbool\\b" << "\\bdynamic_cast\\b" << "\\bint\\b" << "\\bcontinue\\b" << "\\binterface\\b" <<
                           "\\bsigned\\b" << "\\btypename\\b" << "\\bbreak\\b" << "\\belse\\b" << "\\bpackage\\b" << "\\bsynchronized\\b" <<
                           "\\blong\\b" << "\\bsizeof\\b" << "\\bunion\\b" << "\\bcase\\b" << "\\benum\\b" << "\\bstrictfp\\b" <<
                           "\\bmutable\\b" << "\\bstatic\\b" << "\\bunsigned\\b" << "\\bcatch\\b" << "\\btransient\\b" <<
                           "\\bexplicit\\b" << "\\bnamespace\\b" << "\\bstatic_cast\\b" << "\\busing\\b"<< "\\bimplements\\b"<<
                           "\\bchar\\b" << "\\bexport\\b" << "\\bnew\\b" <<"\\bstruct\\b" << "\\bvirtual\\b" << "\\bfinal\\b"<<
                           "\\bextern\\b" << "\\boperator\\b" << "\\bswitch\\b" << "\\bvoid\\b" << "\\binstanceof\\b" <<
                           "\\bcosnt\\b" << "\\bfalse\\b" << "\\bprivate\\b" << "\\btemplate\\b" << "\\bextends\\b"<<
                           "\\bvolatile\\b" << "\\bconst_cast\\b" << "\\bfloat\\b" << "\\bprotected\\b" <<
                           "\\bthis\\b"  << "\\bcontinue\\b" << "\\bfor\\b" << "\\bpublic\\b" << "\\bfinally\\b"<<
                           "\\bthrow\\b" << "\\bwhile\\b" << "\\bdefalut\\b" << "\\btrue\\b" <<
                           "\\bgoto\\b" << "\\btransient\\b" <<"\\btry\\b";
        break;
    }
    case JavaScriptFile:
        keywordPatterns <<"\\bjavascript\\b";
        break;

    default:
        break;

    }

    // Set the attribute of the class format text above
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Set the attribute of the single-line comment
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Set the attribute of the multi-line comment
    multiLineCommentFormat.setForeground(Qt::red);

    // Set the attribute of the quotation
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Set the attribute of the function name
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Set the starting and end form of the comment
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

/*
 * @brief: This function is to highlight the certain string on highlighting keywords list.
 * @param: The pointer to the text to be highlighted.
 * @return: void.
*/

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
