#include "form_message_ui.h"
//----------------------------------------------------------------------------------------------
#include "formatter.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Описатель спецсимвола для вставки
 * Дополнительно см. таблицы символов на http://www.unicode.org/charts/
 */
typedef struct ACharMapItem
{
	const char* Symbol; /*!< \brief Unicode представление символа */
	const char* Name;   /*!< \brief Наименование символа (хинт)   */
} ACharMapItem;
//----------------------------------------------------------------------------------------------
/*!
 * \brief Греческий алфавит
 */
ACharMapItem g_charmap_greece [] =
{
	{"Αα", "альфа"  }, {"Ββ", "бета"  }, {"Γγ", "гамма"  }, {"Δδ", "дельта" },
	{"Εε", "эпсилон"}, {"Ζζ", "дзета" }, {"Ηη", "эта"    }, {"Θθ", "тета"   },
	{"Ιι", "иота"   }, {"Κκ", "каппа" }, {"Λλ", "лямбда" }, {"Μμ", "мю"     },
	{"Νν", "ню"     }, {"Ξξ", "кси"   }, {"Οο", "омикрон"}, {"Ππ", "пи"     },
	{"Ρρ", "ро"     }, {"Σσ", "сигма" }, {"Ττ", "тау"    }, {"Υυ", "ипсилон"},
	{"Φφ", "фи"     }, {"Χχ", "хи"    }, {"Ψψ", "пси"    }, {"Ωω", "омега"  },
	{"Ϝϝ", "дигамма"}, {"Ϛϛ", "стигма"}, {"Ͱͱ", "хета"   }, {"Ϻϻ", "сан"    },
	{"Ϸϸ", "шо"     }, {"Ϙϙ", "коппа" }, {"Ϟϟ", "коппа"  }, {"Ͳͳ", "дисигма"},
	{"Ϡϡ", "дисигма"}
};
//----------------------------------------------------------------------------------------------
/*!
 * \brief Математические символы
 * http://www.unicode.org/charts/ раздел Mathematical Symbols
 */
ACharMapItem g_charmap_math [] =
{
	{"±",   "плюс-минус"},       {"÷",     "деление"},
	{"≡≈≠", "равенство"},        {"≤⩽⩾≥", "сравнение"},
	{"⇒",   "следование"},       {"∈∉",    "принадлежность"},
	{"∅",   "пустое множество"}, {"∞",     "бесконечность"},
	{"√",   "корень"},           {"∫",     "интеграл"},
	{"→",   "функция"}
};
//----------------------------------------------------------------------------------------------
/*!
 * \brief Прочие символы
 */
ACharMapItem g_charmap_other [] =
{
	{"™", "trademark"}, {"©", "copyright"}, {"®", "registered"}
};

//----------------------------------------------------------------------------------------------

FormMessageUI::FormMessageUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	setWindowTitle(QString::fromUtf8("новое сообщение"));

	QIcon icon;
	icon.addFile(":/icons/new16.png",  QSize(16, 16));
	icon.addFile(":/icons/new22.png",  QSize(22, 22));
	icon.addFile(":/icons/new24.png",  QSize(24, 24));
	icon.addFile(":/icons/new32.png",  QSize(32, 32));
	icon.addFile(":/icons/new48.png",  QSize(48, 48));
	icon.addFile(":/icons/new64.png",  QSize(64, 64));
	icon.addFile(":/icons/new128.png", QSize(128, 128));
	setWindowIcon(icon);

	resize(750, 550);
	setMinimumSize(QSize(400, 400));

	m_layout = new QVBoxLayout(this);

	m_toolbar_message = new QToolBar(this);

	m_button_send = new QAction(m_toolbar_message);
	m_button_send->setIcon(QIcon(":/icons/reply22.png"));
	m_button_send->setText(QString::fromUtf8("отправить [CTRL + S]"));
	m_button_send->setShortcut(QString("CTRL+S"));

	m_toolbar_message->addAction(m_button_send);

	m_button_draft = new QAction(m_toolbar_message);
	m_button_draft->setIcon(QIcon(":/icons/draft24.png"));
	m_button_draft->setText(QString::fromUtf8("в черновики [CTRL + D]"));
	m_button_draft->setShortcut(QString("CTRL+D"));

	m_toolbar_message->addAction(m_button_draft);

	m_button_smile = new QAction(m_toolbar_message);
	m_button_smile->setIcon(QIcon(":/icons/smile24.png"));
	m_button_smile->setText(QString::fromUtf8("смайлы"));

	m_menu_smile = new QMenu(m_toolbar_message);

	m_menu_smile_lol = m_menu_smile->addAction(":)))");
	m_menu_smile_lol->setIcon(QIcon(":/smiles/sideways.png"));

	m_menu_smile_biggrin = m_menu_smile->addAction(":))");
	m_menu_smile_biggrin->setIcon(QIcon(":/smiles/biggrin.png"));

	m_menu_smile_smile = m_menu_smile->addAction(":)");
	m_menu_smile_smile->setIcon(QIcon(":/smiles/smile.png"));

	m_menu_smile_frown = m_menu_smile->addAction(":(");
	m_menu_smile_frown->setIcon(QIcon(":/smiles/sad.png"));

	m_menu_smile_wink = m_menu_smile->addAction(";)");
	m_menu_smile_wink->setIcon(QIcon(":/smiles/wink.png"));

	m_menu_smile_smirk = m_menu_smile->addAction(":-\\");
	m_menu_smile_smirk->setIcon(QIcon(":/smiles/pouty.png"));

	m_menu_smile_confused = m_menu_smile->addAction(":???:");
	m_menu_smile_confused->setIcon(QIcon(":/smiles/wacko.png"));

	m_menu_smile_no = m_menu_smile->addAction(":no:");
	m_menu_smile_no->setIcon(QIcon(":/smiles/stop.png"));

	m_menu_smile_sup = m_menu_smile->addAction(":up:");
	m_menu_smile_sup->setIcon(QIcon(":/smiles/thumbs_up.png"));

	m_menu_smile_down = m_menu_smile->addAction(":down:");
	m_menu_smile_down->setIcon(QIcon(":/smiles/thumb_down.png"));

	m_menu_smile_super = m_menu_smile->addAction(":super:");
	m_menu_smile_super->setIcon(QIcon(":/smiles/cool.png"));

	m_menu_smile_shuffle = m_menu_smile->addAction(":shuffle:");
	m_menu_smile_shuffle->setIcon(QIcon(":/smiles/blush.png"));

	m_menu_smile_wow = m_menu_smile->addAction(":wow:");
	m_menu_smile_wow->setIcon(QIcon(":/smiles/shock.png"));

	m_menu_smile_crash = m_menu_smile->addAction(":crash:");
	m_menu_smile_crash->setIcon(QIcon(":/smiles/angry.png"));

	m_menu_smile_user = m_menu_smile->addAction(":user:");
	m_menu_smile_user->setIcon(QIcon(":/smiles/music.png"));

	m_menu_smile_maniac = m_menu_smile->addAction(":maniac:");
	m_menu_smile_maniac->setIcon(QIcon(":/smiles/devil.png"));

	m_menu_smile_xz = m_menu_smile->addAction(":xz:");
	m_menu_smile_xz->setIcon(QIcon(":/smiles/wrong.png"));

	m_menu_smile_beer = m_menu_smile->addAction(":beer:");
	m_menu_smile_beer->setIcon(QIcon(":/smiles/beer.png"));

	m_button_smile->setMenu(m_menu_smile);

	m_toolbar_message->addAction(m_button_smile);

	m_button_bold = new QAction(m_toolbar_message);
	m_button_bold->setIcon(QIcon(":/icons/bold24.png"));
	m_button_bold->setText(QString::fromUtf8("жирный [CTRL + B]"));
	m_button_bold->setShortcut(QString("Ctrl+B"));
	m_toolbar_message->addAction(m_button_bold);

	m_button_italic = new QAction(m_toolbar_message);
	m_button_italic->setIcon(QIcon(":/icons/italic24.png"));
	m_button_italic->setText(QString::fromUtf8("курсив [CTRL + I]"));
	m_button_italic->setShortcut(QString("Ctrl+I"));
	m_toolbar_message->addAction(m_button_italic);

	m_button_quote = new QAction(m_toolbar_message);
	m_button_quote->setIcon(QIcon(":/icons/quote24.png"));
	m_button_quote->setText(QString::fromUtf8("цитирование [CTRL + Q]"));
	m_button_quote->setShortcut(QString("Ctrl+Q"));
	m_toolbar_message->addAction(m_button_quote);

	m_button_image = new QAction(m_toolbar_message);
	m_button_image->setIcon(QIcon(":/icons/image24.png"));
	m_button_image->setText(QString::fromUtf8("рисунок [CTRL + G]"));
	m_button_image->setShortcut(QString("Ctrl+G"));
	m_toolbar_message->addAction(m_button_image);

	m_button_hyperlink = new QAction(m_toolbar_message);
	m_button_hyperlink->setIcon(QIcon(":/icons/hyperlink24.png"));
	m_button_hyperlink->setText(QString::fromUtf8("ссылка [CTRL + H]"));
	m_button_hyperlink->setShortcut(QString("Ctrl+H"));
	m_toolbar_message->addAction(m_button_hyperlink);

	m_button_code = new QAction(m_toolbar_message);
	m_button_code->setIcon(QIcon(":/icons/code24.png"));
	m_button_code->setText(QString::fromUtf8("код [CTRL + O]"));
	m_button_code->setShortcut(QString("Ctrl+O"));

	m_menu_code = new QMenu(m_toolbar_message);

	m_menu_code_code    = m_menu_code->addAction("[code]");
	m_menu_code_asm     = m_menu_code->addAction("[asm]");
	m_menu_code_ccode   = m_menu_code->addAction("[ccode]");
	m_menu_code_c       = m_menu_code->addAction("[c]");
	m_menu_code_csharp_ = m_menu_code->addAction("[c#]");
	m_menu_code_csharp  = m_menu_code->addAction("[csharp]");
	m_menu_code_cs      = m_menu_code->addAction("[cs]");
	m_menu_code_msil    = m_menu_code->addAction("[msil]");
	m_menu_code_midl    = m_menu_code->addAction("[midl]");
	m_menu_code_pascal  = m_menu_code->addAction("[pascal]");
	m_menu_code_vb      = m_menu_code->addAction("[vb]");
	m_menu_code_sql     = m_menu_code->addAction("[sql]");
	m_menu_code_perl    = m_menu_code->addAction("[perl]");
	m_menu_code_php     = m_menu_code->addAction("[php]");
	m_menu_code_java    = m_menu_code->addAction("[java]");
	m_menu_code_xml     = m_menu_code->addAction("[xml]");
	m_menu_code_lisp    = m_menu_code->addAction("[lisp]");
	m_menu_code_haskell = m_menu_code->addAction("[haskell]");

	m_button_code->setMenu(m_menu_code);
	m_toolbar_message->addAction(m_button_code);

	m_button_char = new QAction(m_toolbar_message);
	m_button_char->setIcon(QIcon(":/icons/char24.png"));
	m_button_char->setText(QString::fromUtf8("спецсимволы"));

	m_menu_char = new QMenu(m_toolbar_message);

	QMenu* submenu_char = m_menu_char->addMenu(QString::fromUtf8("Греческий"));

	for (size_t i = 0; i < sizeof(g_charmap_greece) / sizeof(ACharMapItem); i++)
	{
		QString symbol = QString::fromUtf8(g_charmap_greece[i].Symbol);
		QString name   = QString::fromUtf8(g_charmap_greece[i].Name);

		QAction* action;

		if (name.length() != 0)
			action = submenu_char->addAction(symbol + " (" + name + ")");
		else
			action = submenu_char->addAction(symbol);

		action->setToolTip(symbol);
	}

	submenu_char = m_menu_char->addMenu(QString::fromUtf8("Математика"));

	for (size_t i = 0; i < sizeof(g_charmap_math) / sizeof(ACharMapItem); i++)
	{
		QString symbol = QString::fromUtf8(g_charmap_math[i].Symbol);
		QString name   = QString::fromUtf8(g_charmap_math[i].Name);

		QAction* action;

		if (name.length() != 0)
			action = submenu_char->addAction(symbol + " (" + name + ")");
		else
			action = submenu_char->addAction(symbol);

		action->setToolTip(symbol);
	}

	submenu_char = m_menu_char->addMenu(QString::fromUtf8("Прочее"));

	for (size_t i = 0; i < sizeof(g_charmap_other) / sizeof(ACharMapItem); i++)
	{
		QString symbol = QString::fromUtf8(g_charmap_other[i].Symbol);
		QString name   = QString::fromUtf8(g_charmap_other[i].Name);

		QAction* action;

		if (name.length() != 0)
			action = submenu_char->addAction(symbol + " (" + name + ")");
		else
			action = submenu_char->addAction(symbol);

		action->setToolTip(symbol);
	}

	m_button_char->setMenu(m_menu_char);
	m_toolbar_message->addAction(m_button_char);

	m_layout->addWidget(m_toolbar_message);

	m_layout_subject = new QHBoxLayout();

	m_label_subject = new QLabel(this);
	m_label_subject->setText(QString::fromUtf8("тема"));

	m_layout_subject->addWidget(m_label_subject);

	m_text_subject = new QLineEdit(this);
	m_text_subject->setMaxLength(128);
	m_text_subject->setText(QString::fromUtf8("(тема)"));

	m_layout_subject->addWidget(m_text_subject);

	m_layout->addLayout(m_layout_subject);

	m_tab_message = new QTabWidget(this);

	m_layout->addWidget(m_tab_message);

	m_tab_source = new QWidget();
	m_tab_message->addTab(m_tab_source, QString::fromUtf8("сообщение"));

	m_tab_view = new QWidget();
	m_tab_message->addTab(m_tab_view, QString::fromUtf8("предпросмотр"));

	m_layout_source = new QHBoxLayout(m_tab_source);
	m_layout_view   = new QHBoxLayout(m_tab_view);

	m_text_source = new ASpellTextEdit(m_tab_source);

#ifndef Q_WS_WIN
	m_text_source->setFrameShadow(QFrame::Plain);
#endif

	m_text_source->setUndoRedoEnabled(true);
	m_text_source->setAcceptRichText(false);
	m_text_source->setTextInteractionFlags(Qt::TextEditorInteraction);

	QFont font;
	font.setFamily(QString::fromUtf8("Courier New"));
	font.setPointSize(10);
	m_text_source->setFont(font);

	m_layout_source->addWidget(m_text_source);

	m_text_view = new AWebViewWidget(m_tab_view);

	m_layout_view->addWidget(m_text_view);

	connect(m_button_smile, SIGNAL(triggered()), this, SLOT(button_smile_triggered()));

	connect(m_menu_smile_lol,      SIGNAL(triggered()), this, SLOT(menu_smile_lol_triggered()));      // ":)))"
	connect(m_menu_smile_biggrin,  SIGNAL(triggered()), this, SLOT(menu_smile_biggrin_triggered()));  // ":))"
	connect(m_menu_smile_smile,    SIGNAL(triggered()), this, SLOT(menu_smile_smile_triggered()));    // ":)"
	connect(m_menu_smile_frown,    SIGNAL(triggered()), this, SLOT(menu_smile_frown_triggered()));    // ":("
	connect(m_menu_smile_wink,     SIGNAL(triggered()), this, SLOT(menu_smile_wink_triggered()));     // ";)"
	connect(m_menu_smile_smirk,    SIGNAL(triggered()), this, SLOT(menu_smile_smirk_triggered()));    // ":-\\"
	connect(m_menu_smile_confused, SIGNAL(triggered()), this, SLOT(menu_smile_confused_triggered())); // ":???:"
	connect(m_menu_smile_no,       SIGNAL(triggered()), this, SLOT(menu_smile_no_triggered()));       // ":no:"
	connect(m_menu_smile_sup,      SIGNAL(triggered()), this, SLOT(menu_smile_sup_triggered()));      // ":up:"
	connect(m_menu_smile_down,     SIGNAL(triggered()), this, SLOT(menu_smile_down_triggered()));     // ":down:"
	connect(m_menu_smile_super,    SIGNAL(triggered()), this, SLOT(menu_smile_super_triggered()));    // ":super:"
	connect(m_menu_smile_shuffle,  SIGNAL(triggered()), this, SLOT(menu_smile_shuffle_triggered()));  // ":shuffle:"
	connect(m_menu_smile_wow,      SIGNAL(triggered()), this, SLOT(menu_smile_wow_triggered()));      // ":wow:"
	connect(m_menu_smile_crash,    SIGNAL(triggered()), this, SLOT(menu_smile_crash_triggered()));    // ":crash:"
	connect(m_menu_smile_user,     SIGNAL(triggered()), this, SLOT(menu_smile_user_triggered()));     // ":user:"
	connect(m_menu_smile_maniac,   SIGNAL(triggered()), this, SLOT(menu_smile_maniac_triggered()));   // ":maniac:"
	connect(m_menu_smile_xz,       SIGNAL(triggered()), this, SLOT(menu_smile_xz_triggered()));       // ":xz:"
	connect(m_menu_smile_beer,     SIGNAL(triggered()), this, SLOT(menu_smile_beer_triggered()));     // ":beer:"

	connect(m_button_bold,      SIGNAL(triggered()), this, SLOT(button_bold_triggered()));
	connect(m_button_italic,    SIGNAL(triggered()), this, SLOT(button_italic_triggered()));
	connect(m_button_quote,     SIGNAL(triggered()), this, SLOT(button_quote_triggered()));
	connect(m_button_image,     SIGNAL(triggered()), this, SLOT(button_image_triggered()));
	connect(m_button_hyperlink, SIGNAL(triggered()), this, SLOT(button_hyperlink_triggered()));
	connect(m_button_code,      SIGNAL(triggered()), this, SLOT(button_code_triggered()));

	connect(m_menu_code_code,    SIGNAL(triggered()), this, SLOT(menu_code_code_triggered()));
	connect(m_menu_code_asm,     SIGNAL(triggered()), this, SLOT(menu_code_asm_triggered()));
	connect(m_menu_code_ccode,   SIGNAL(triggered()), this, SLOT(menu_code_ccode_triggered()));
	connect(m_menu_code_c,       SIGNAL(triggered()), this, SLOT(menu_code_c_triggered()));
	connect(m_menu_code_csharp_, SIGNAL(triggered()), this, SLOT(menu_code_csharp__triggered()));
	connect(m_menu_code_csharp,  SIGNAL(triggered()), this, SLOT(menu_code_csharp_triggered()));
	connect(m_menu_code_cs,      SIGNAL(triggered()), this, SLOT(menu_code_cs_triggered()));
	connect(m_menu_code_msil,    SIGNAL(triggered()), this, SLOT(menu_code_msil_triggered()));
	connect(m_menu_code_midl,    SIGNAL(triggered()), this, SLOT(menu_code_midl_triggered()));
	connect(m_menu_code_pascal,  SIGNAL(triggered()), this, SLOT(menu_code_pascal_triggered()));
	connect(m_menu_code_vb,      SIGNAL(triggered()), this, SLOT(menu_code_vb_triggered()));
	connect(m_menu_code_sql,     SIGNAL(triggered()), this, SLOT(menu_code_sql_triggered()));
	connect(m_menu_code_perl,    SIGNAL(triggered()), this, SLOT(menu_code_perl_triggered()));
	connect(m_menu_code_php,     SIGNAL(triggered()), this, SLOT(menu_code_php_triggered()));
	connect(m_menu_code_java,    SIGNAL(triggered()), this, SLOT(menu_code_java_triggered()));
	connect(m_menu_code_xml,     SIGNAL(triggered()), this, SLOT(menu_code_xml_triggered()));
	connect(m_menu_code_lisp,    SIGNAL(triggered()), this, SLOT(menu_code_lisp_triggered()));
	connect(m_menu_code_haskell, SIGNAL(triggered()), this, SLOT(menu_code_haskell_triggered()));

	connect(m_button_char, SIGNAL(triggered()), this, SLOT(button_char_triggered()));

	connect(m_menu_char, SIGNAL(triggered(QAction*)), this, SLOT(menu_char_triggered(QAction*)));

	// восстановление layout
	restore();
}
//----------------------------------------------------------------------------------------------

FormMessageUI::~FormMessageUI ()
{
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::save ()
{
	QSettings settings;

	if (windowState() == Qt::WindowNoState)
	{
		QRect rect = geometry();

		settings.setValue("form_message/left",   rect.left());
		settings.setValue("form_message/top",    rect.top());
		settings.setValue("form_message/width",  rect.width());
		settings.setValue("form_message/height", rect.height());
	}

	settings.setValue("form_message/state", static_cast<int>(windowState()));
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::restore ()
{
	QSettings settings;

	QRect screen = QDesktopWidget().screenGeometry(this);

	int w = settings.value("form_message/width",  width()).toInt();
	int h = settings.value("form_message/height", height()).toInt();
	int x = settings.value("form_message/left",   (screen.width()  - w) / 2).toInt();
	int y = settings.value("form_message/top",    (screen.height() - h) / 2).toInt();

	// setGeometry как-то некорректно работает :(
	resize(w, h);
	move(x, y);

	Qt::WindowStates state = static_cast<Qt::WindowStates>(settings.value("form_message/state", Qt::WindowNoState).toInt());

	setWindowState(state);
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_smile_triggered ()
{
	QWidget* widget = m_toolbar_message->widgetForAction(m_button_smile);
	m_menu_smile->exec(widget->mapToGlobal(QPoint(0, widget->height())));
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_lol_triggered () // ":)))"
{
	m_text_source->insertPlainText(":)))");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_biggrin_triggered () // ":))"
{
	m_text_source->insertPlainText(":))");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_smile_triggered () // ":)"
{
	m_text_source->insertPlainText(":)");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_frown_triggered () // ":("
{
	m_text_source->insertPlainText(":(");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_wink_triggered () // ";)"
{
	m_text_source->insertPlainText(";)");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_smirk_triggered () // ":-\"
{
	m_text_source->insertPlainText(":-\\");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_confused_triggered () // ":???:"
{
	m_text_source->insertPlainText(":???:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_no_triggered () // ":no:"
{
	m_text_source->insertPlainText(":no:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_sup_triggered () // ":up:"
{
	m_text_source->insertPlainText(":up:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_down_triggered () // ":down:"
{
	m_text_source->insertPlainText(":down:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_super_triggered () // ":super:"
{
	m_text_source->insertPlainText(":super:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_shuffle_triggered () // ":shuffle:"
{
	m_text_source->insertPlainText(":shuffle:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_wow_triggered () // ":wow:"
{
	m_text_source->insertPlainText(":wow:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_crash_triggered () // ":crash:"
{
	m_text_source->insertPlainText(":crash:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_user_triggered () // ":user:"
{
	m_text_source->insertPlainText(":user:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_maniac_triggered () // ":maniac:"
{
	m_text_source->insertPlainText(":maniac:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_xz_triggered () // ":xz:"
{
	m_text_source->insertPlainText(":xz:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_smile_beer_triggered () // ":beer:"
{
	m_text_source->insertPlainText(":beer:");
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_bold_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[b]" + cursor.selectedText() + "[/b]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_italic_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[i]" + cursor.selectedText() + "[/i]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_quote_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[q]" + cursor.selectedText() + "[/q]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_image_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[img]" + cursor.selectedText() + "[/img]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_hyperlink_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();

	QString text = cursor.selectedText();

	int url = AParser::isURL(text);

	if (url == 1)
		cursor.insertText((QString)"[url=" + cursor.selectedText() + QString::fromUtf8("]здесь[/url]"));
	else
		cursor.insertText((QString)"[url=]" + cursor.selectedText() + "[/url]");

	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_code_triggered ()
{
	QWidget* widget = m_toolbar_message->widgetForAction(m_button_code);
	m_menu_code->exec(widget->mapToGlobal(QPoint(0, widget->height())));
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_code_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[code]" + cursor.selectedText() + "[/code]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_asm_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[asm]" + cursor.selectedText() + "[/asm]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_ccode_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[]" + cursor.selectedText() + "[/]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_c_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[c]" + cursor.selectedText() + "[/c]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_csharp__triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[c#]" + cursor.selectedText() + "[/c#]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_csharp_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[csharp]" + cursor.selectedText() + "[/csharp]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_cs_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[cs]" + cursor.selectedText() + "[/cs]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_msil_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[msil]" + cursor.selectedText() + "[/msil]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_midl_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[midl]" + cursor.selectedText() + "[/midl]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_pascal_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[pascal]" + cursor.selectedText() + "[/pascal]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_vb_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[vb]" + cursor.selectedText() + "[/vb]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_sql_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[sql]" + cursor.selectedText() + "[/sql]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_perl_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[perl]" + cursor.selectedText() + "[/perl]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_php_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[php]" + cursor.selectedText() + "[/php]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_java_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[java]" + cursor.selectedText() + "[/java]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_xml_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[xml]" + cursor.selectedText() + "[/xml]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_lisp_triggered ()
{

	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[lisp]" + cursor.selectedText() + "[/lisp]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_code_haskell_triggered ()
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText((QString)"[haskell]" + cursor.selectedText() + "[/haskell]");
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::button_char_triggered ()
{
	QWidget* widget = m_toolbar_message->widgetForAction(m_button_char);
	m_menu_char->exec(widget->mapToGlobal(QPoint(0, widget->height())));
}
//----------------------------------------------------------------------------------------------

void FormMessageUI::menu_char_triggered (QAction* action)
{
	QTextCursor cursor = m_text_source->textCursor();
	cursor.insertText(action->toolTip());
	cursor.clearSelection();
}
//----------------------------------------------------------------------------------------------
