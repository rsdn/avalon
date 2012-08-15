#ifndef _avalon_parser_test_h_
#define _avalon_parser_test_h_

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QApplication>
#include <QCoreApplication>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "../../src/parser.h"

class AParser_Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(AParser_Test);
	CPPUNIT_TEST(parseQuote_test);
	CPPUNIT_TEST(parseQuotes_test);
	CPPUNIT_TEST(parseBlocks_test);
	CPPUNIT_TEST_SUITE_END();

	public:

		// тест вычисления квоты строки
		void parseQuote_test  ();
		// тест вычисления квот нескольких строк
		void parseQuotes_test ();
		// тест парсинга блоков
		void parseBlocks_test ();

	private:

		std::string std_string (const QString& source)                          { return std::string(source.toUtf8().constData());    }
		std::string std_string (const char* source)                             { return std::string(source);                         }
		std::string make_msg   (const char* source1,    const char* source2)    { return std::string(source1) + std::string(source2); }
		std::string make_msg   (const char* source1,    const QString& source2) { return std::string(source1) + std_string(source2);  }
		std::string make_msg   (const QString& source1, const QString& source2) { return std_string(source1)  + std_string(source2);  }
		QString make_special   (const char* source)                             { return make_special(QString::fromUtf8(source));     }
		QString make_special   (const QString& source)
		{
			QString data = source;

			data.replace("&", "&amp;");
			data.replace(">", "&gt;");
			data.replace("<", "&lt;");

			data.replace("&amp;#", "&#");

			return data;
		}

		/*!
		 * \brief Отладочная функция преобразования списка монолитного блока в строку
		 * \param block Исходный монолитный блок
		 * \return Отладочная строка с содержимым блока
		 */
		QString ParsedBlockToString (const AParsedBlock& block, const QString& pad = "");

		/*!
		 * \brief Отладочная функция преобразования списка монолитных блоков в строку
		 * \param list Исходный список монолитных блоков
		 * \return Отладочная строка с содержимым блоков
		 */
		QString ParsedBlockListToString (const AParsedBlockList& list, const QString& pad = "");
};

#endif   // _avalon_parser_test_h_
