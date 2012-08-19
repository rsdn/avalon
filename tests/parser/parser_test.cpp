#include "parser_test.h"
//----------------------------------------------------------------------------------------------
/*!
 * (no comments)
 * \param argc (no comments)
 * \param argv (no comments)
 * \return (no comments)
 */
int main (int argc, char** argv)
{
	QApplication app(argc, argv);

	CppUnit::Test* suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);

	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));

	bool sucess = runner.run();

	return sucess ? 0 : 1;
}
//----------------------------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(AParser_Test);
//----------------------------------------------------------------------------------------------

void AParser_Test::parseQuote_test ()
{
	typedef struct ATestEntry
	{
		const char* Source;
		int         QuoteLevel;
		const char* QuoteText;
		const char* Data;
	} ATestEntry;

	ATestEntry test_data [] =
	{
		// source, level, quote, data
		{">",   1, ">",   ""},
		{">>",  2, ">>",  ""},
		{">>>", 3, ">>>", ""},

		{"> ",     1, ">",   " "},
		{"> > ",   2, ">>",  " "},
		{"> > > ", 3, ">>>", " "},

		{"ААА> ", 1, "ААА>", " "},

		{"ААА> Это>тест",  2, "ААА>Это>", "тест"},
		{"ААА> Этот>тест", 1, "ААА>",     " Этот>тест"},

		{" >",   0, "", " >"},
		{"  А>", 0, "", "  А>"},
		{"A  >", 0, "", "A  >"},
		{"A А>", 0, "", "A А>"},
	};

	for (size_t i = 0; i < sizeof(test_data) / sizeof(ATestEntry); i++)
	{
		QString source     = make_special(test_data[i].Source);
		QString quote_text = make_special(test_data[i].QuoteText);
		QString data       = make_special(test_data[i].Data);

		AQuotedString result = AParser::parseQuote(source);

		CPPUNIT_ASSERT_EQUAL_MESSAGE(make_msg("Assert quote level for string:\n", source), test_data[i].QuoteLevel,  result.QuoteLevel);
		CPPUNIT_ASSERT_EQUAL_MESSAGE(make_msg("Assert quote text for string:\n",  source), std_string(quote_text),   std_string(result.QuoteText));
		CPPUNIT_ASSERT_EQUAL_MESSAGE(make_msg("Assert data for string:\n",        source), std_string(data),         std_string(result.Data));
	}
}
//----------------------------------------------------------------------------------------------

void AParser_Test::parseQuotes_test ()
{
	QString source;

	source += QString::fromUtf8(">> Мама \r\n");
	source += QString::fromUtf8(">>>  Мыла \n");
	source += QString::fromUtf8(">>Раму ");

	source = make_special(source);

	AQuotedStringList result = AParser::parseQuotes(source);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("list count error", 3, result.count());

	AQuotedString line_1 = result[0];
	AQuotedString line_2 = result[1];
	AQuotedString line_3 = result[2];

	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert quote level for string 1"), 2,                                  line_1.QuoteLevel);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert quote text for string 1"),  std_string(make_special(">>")),     std_string(line_1.QuoteText));
	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert data for string 1"),        std_string(make_special(" Мама ")), std_string(line_1.Data));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert quote level for string 2"), 3,                                   line_2.QuoteLevel);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert quote text for string 2"),  std_string(make_special(">>>")),     std_string(line_2.QuoteText));
	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert data for string 2"),        std_string(make_special("  Мыла ")), std_string(line_2.Data));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert quote level for string 3"), 2,                                  line_3.QuoteLevel);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert quote text for string 3"),  std_string(make_special(">>")),     std_string(line_3.QuoteText));
	CPPUNIT_ASSERT_EQUAL_MESSAGE(std_string("Assert data for string 3"),        std_string(make_special("Раму ")),  std_string(line_3.Data));
}
//----------------------------------------------------------------------------------------------

void AParser_Test::parseBlocks_test ()
{
	QString test_path = QCoreApplication::applicationDirPath() + "/../tests";

	QDir test_dir(test_path);

	QStringList name_filters;
	name_filters << "parseBlocks_*.src";

	QStringList source_list = test_dir.entryList(name_filters, QDir::Files, QDir::Name);

	for (int i = 0; i < source_list.count(); i++)
	{
		QString source;
		QString target;

		QString source_file_name = test_path + "/" + source_list[i];
		QString target_file_name = source_file_name.mid(0, source_file_name.length() - 3) + "tgt";

		QFile source_file(source_file_name);

		if (source_file.open(QIODevice::ReadOnly) == true)
		{
			QTextStream in(&source_file);
			in.setCodec("UTF-8");
			source = in.readAll();
			source_file.close();
			source = make_special(source);
		}
		else
		{
			CPPUNIT_FAIL(make_msg("Can not open source test file ", source_file_name));
			continue;
		}

		QFile target_file(target_file_name);

		if (target_file.open(QIODevice::ReadOnly) == true)
		{
			QTextStream in(&target_file);
			in.setCodec("UTF-8");
			target = in.readAll();
			target_file.close();
			target = make_special(target);
		}
		else
		{
			CPPUNIT_FAIL(make_msg("Can not open target test file ", target_file_name));
			continue;
		}

		AParsedBlockList list = AParser::parseBlocks(source);

		QString compare = ParsedBlockListToString(list);

		if (compare != target)
		{
			QString message;
			message += "test file: " + source_file_name + "\n";
			message += "--------------------\n";
			message += "source file content:\n";
			message += "--------------------\n";
			message += source;
			message += "--------------------\n";
			message += "target file content:\n";
			message += "--------------------\n";
			message += target;
			message += "--------------------\n";
			message += "real parse content:\n";
			message += "--------------------\n";
			message += compare;
			message += "--------------------\n";

			CPPUNIT_FAIL(std_string(message));
		}
	}
}
//----------------------------------------------------------------------------------------------

QString AParser_Test::ParsedBlockToString (const AParsedBlock& block, const QString& pad)
{
	QString pad2 = "";

	if (block.SubBlocks.count() > 0)
		pad2 = "  ";

	QString result = "";

	result += "AParsedBlock\n";
	result += pad + "(\n";
	result += pad + "	Type    " + pad2 + "= ";

	switch (block.Type)
	{
		case pbtText:      result += "pbtText";      break;
		case pbtCode:      result += "pbtCode";      break;
		case pbtAssembler: result += "pbtAssembler"; break;
		case pbtC:         result += "pbtC";         break;
		case pbtCPP:       result += "pbtCPP";       break;
		case pbtCSharp:    result += "pbtCSharp";    break;
		case pbtMSIL:      result += "pbtMSIL";      break;
		case pbtIDL:       result += "pbtIDL";       break;
		case pbtPascal:    result += "pbtPascal";    break;
		case pbtBasic:     result += "pbtBasic";     break;
		case pbtSQL:       result += "pbtSQL";       break;
		case pbtPerl:      result += "pbtPerl";      break;
		case pbtPHP:       result += "pbtPHP";       break;
		case pbtJava:      result += "pbtJava";      break;
		case pbtXML:       result += "pbtXML";       break;
		case pbtLisp:      result += "pbtLisp";      break;
		case pbtHaskell:   result += "pbtHaskell";   break;
		case pbtErlang:    result += "pbtErlang";    break;
		case pbtOCaml:     result += "pbtOCaml";     break;
		case pbtProlog:    result += "pbtProlog";    break;
		case pbtPython:    result += "pbtPython";    break;
		case pbtRuby:      result += "pbtRuby";      break;
		case pbtTagline:   result += "pbtTagline";   break;
		case pbtModerator: result += "pbtModerator"; break;
		case pbtQuote:     result += "pbtQuote";     break;
		case pbtTable:     result += "pbtTable";     break;
		case pbtCut:       result += "pbtCut";       break;
		default:           result += "default";
	}

	result += "\n";
	result += pad + "	SubType " + pad2 + "= ";

	switch (block.SubType)
	{
		case pbtText:   result += "pbstText";       break;
		case pbtCode:   result += "pbstSourceCode"; break;
		case pbstBlock: result += "pbstBlock";      break;
		default:        result += "default";
	}

	result += "\n";

	if (block.Title.length() > 0)
		result += pad + "	Title   " + pad2 + "= " + block.Title + "\n";

	if (block.Strings.count() > 0)
	{
		result += pad + "	Strings " + pad2 + "= AQuotedStringList\n";
		result += pad + "	(\n";

		for (int i = 0; i < block.Strings.count(); i++)
		{
			AQuotedString string = block.Strings[i];

			result += pad + "		[" + QString::number(i) + "] = AQuotedString\n";
			result += pad + "		(\n";
			result += pad + "			QuoteLevel = " + QString::number(string.QuoteLevel) + "\n";

			if (string.QuoteText.length() > 0)
				result += pad + "			QuoteText  = " + string.QuoteText + "\n";

			result += pad + "			Data       = " + string.Data + "\n";
			result += pad + "		)\n";
		}

		result += pad + "	)\n";
	}

	if (block.SubBlocks.count() > 0)
		result += pad + "	SubBlocks = " + ParsedBlockListToString(block.SubBlocks, pad + "	");

	result += pad + ")\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AParser_Test::ParsedBlockListToString (const AParsedBlockList& list, const QString& pad)
{
	QString result;

	if (list.count() > 0)
	{
		result += "AParsedBlockList\n";
		result += pad + "(\n";

		for (int i = 0; i < list.count(); i++)
			result += pad + "	[" + QString::number(i) + "] = " + ParsedBlockToString(list[i], pad + "	");

		result += pad + ")\n";
	}

	return result;
}
//----------------------------------------------------------------------------------------------

void AParser_Test::isURL_test ()
{
	typedef struct ATestEntry
	{
		const char* Source;
		int         Result;
	} ATestEntry;

	ATestEntry test_data [] =
	{
		{ "http://yandex.ru",    1 },
		{ "https://yandex.ru",   1 },
		{ "yandex.ru",           1 },
		{ "YANDEX.RU",           1 },
		{ "мчс.рф",              1 },
		{ "МЧС.РФ",              1 },
		{ "здесь",               0 },
		{ "тут",                 0 },
		{ "somewhere",           0 },
		{ "data://yandex.ru",    2 },
		{ "/etc/passwd",         2 },
		{ "file:///etc/passwd",  2 },
	};

	for (size_t i = 0; i < sizeof(test_data) / sizeof(ATestEntry); i++)
	{
		QString source = QString::fromUtf8(test_data[i].Source);

		int result = AParser::isURL(source);

		CPPUNIT_ASSERT_EQUAL_MESSAGE(make_msg("Assert is URL:\n", source + " (" + QUrl::fromUserInput(source).toString() + ")"), test_data[i].Result, result);
	}
}
//----------------------------------------------------------------------------------------------
