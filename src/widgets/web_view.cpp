#include "web_view.h"
//----------------------------------------------------------------------------------------------

AWebView::AWebView (QWidget* parent) : QWebView (parent)
{
	LinkHovered = false;

	setPage(new AWebPage(this));

	page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

	pageAction(QWebPage::OpenLink)->setVisible(false);
	pageAction(QWebPage::OpenLinkInNewWindow)->setVisible(false);
	pageAction(QWebPage::OpenFrameInNewWindow)->setVisible(false);
	pageAction(QWebPage::DownloadLinkToDisk)->setVisible(false);
	pageAction(QWebPage::OpenImageInNewWindow)->setVisible(false);
	pageAction(QWebPage::DownloadImageToDisk)->setVisible(false);
	pageAction(QWebPage::CopyImageToClipboard)->setVisible(false);
	pageAction(QWebPage::Reload)->setVisible(false);

	pageAction(QWebPage::CopyLinkToClipboard)->setText(QString::fromUtf8("Копировать ссылку"));
	pageAction(QWebPage::CopyLinkToClipboard)->setIcon(QIcon(":/icons/copy16.png"));

	pageAction(QWebPage::Copy)->setText(QString::fromUtf8("Копировать"));
	pageAction(QWebPage::Copy)->setIcon(QIcon(":/icons/copy16.png"));
	pageAction(QWebPage::Copy)->setShortcuts(QKeySequence::Copy);

	settings()->setDefaultTextEncoding("utf-8");

	settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
	settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);

	// полезно для отладки генерируемого HTML
	settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
};
//----------------------------------------------------------------------------------------------

void AWebView::keyPressEvent (QKeyEvent* event)
{
	if (event->matches(QKeySequence::Copy) == true)
		pageAction(QWebPage::Copy)->trigger();
	else
		QWebView::keyPressEvent(event);
}
//----------------------------------------------------------------------------------------------

void AWebView::contextMenuEvent (QContextMenuEvent* event)
{
	QString selected = page()->selectedText();

	if (selected.length() == 0 || LinkHovered == true)
		QWebView::contextMenuEvent(event);
	else
	{
		// Странно, но если делать m_menu переменной класса и задать все нужные свойства в конструкторе,
		// то возникает AV :\ разбираться с причинами лениво, ибо не так уж и накладно создавать каждый раз

		QMenu* m_menu = new QMenu(this);

		m_menu->addAction(pageAction(QWebPage::Copy));

		m_menu->addSeparator();

		QAction* yandex = m_menu->addAction(QString::fromUtf8("Яндекс"));
		yandex->setIcon(QIcon(":/icons/yandex.ico"));

		QAction* wikipedia = m_menu->addAction(QString::fromUtf8("Википедия"));
		wikipedia->setIcon(QIcon(":/icons/wikipedia.ico"));

		QAction* slovari_yandex = m_menu->addAction(QString::fromUtf8("Яндекс.Словари"));
		slovari_yandex->setIcon(QIcon(":/icons/slovari.yandex.ico"));

		QAction* google = m_menu->addAction(QString::fromUtf8("Google"));
		google->setIcon(QIcon(":/icons/google.ico"));

		QAction* google_translate = m_menu->addAction(QString::fromUtf8("Google Переводчик"));
		google_translate->setIcon(QIcon(":/icons/google.translate.ico"));

		QAction* rsdn = m_menu->addAction(QString::fromUtf8("RSDN"));
		rsdn->setIcon(QIcon(":/icons/rsdn16.png"));

		connect(yandex,           SIGNAL(triggered()), this, SLOT(menu_yandex_triggered()));
		connect(wikipedia,        SIGNAL(triggered()), this, SLOT(menu_wikipedia_triggered()));
		connect(slovari_yandex,   SIGNAL(triggered()), this, SLOT(menu_slovari_yandex_triggered()));
		connect(google,           SIGNAL(triggered()), this, SLOT(menu_google_triggered()));
		connect(google_translate, SIGNAL(triggered()), this, SLOT(menu_google_translate_triggered()));
		connect(rsdn,             SIGNAL(triggered()), this, SLOT(menu_rsdn_triggered()));

		m_menu->exec(event->globalPos());

		delete m_menu;
	}
}
//----------------------------------------------------------------------------------------------

void AWebView::menu_yandex_triggered ()
{
	QString selected = page()->selectedText();

	QString url = (QString)"http://yandex.ru/yandsearch?text=" + selected;

	QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void AWebView::menu_wikipedia_triggered ()
{
	QString selected = page()->selectedText();

	QString url = (QString)"https://ru.wikipedia.org/wiki/" + selected;

	QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void AWebView::menu_slovari_yandex_triggered ()
{
	QString selected = page()->selectedText();

	QString url = (QString)"http://lingvo.yandex.ru/en?st_translate=on&text=" + selected;

	QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void AWebView::menu_google_triggered ()
{
	QString selected = page()->selectedText();

	QString url = (QString)"https://www.google.ru/search?hl=ru&q=" + selected;

	QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void AWebView::menu_google_translate_triggered ()
{
	QString selected = page()->selectedText();

	// примитивная проверка на то, что текст английский
	int is_english = 0;

	for (int i = 0; i < selected.length(); i++)
		if (selected[i].unicode() < 128)
			is_english++;

	QString url;

	if (is_english >= selected.length() / 2)
		url = (QString)"https://translate.google.com/#auto/ru/" + selected;
	else
		url = (QString)"https://translate.google.com/#ru/en/" + selected;

	QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void AWebView::menu_rsdn_triggered ()
{
	QString selected = page()->selectedText();

	QString url = (QString)"https://rsdn.ru/rsdnsearch?text=" + selected;

	QDesktopServices::openUrl(url);
}

//----------------------------------------------------------------------------------------------
// AWebViewWidget
//----------------------------------------------------------------------------------------------

AWebViewWidget::AWebViewWidget (QWidget* parent) : QFrame (parent)
{
	View = new AWebView(this);
	View->setGeometry(2, 2, width() - 4, height() - 4);
	View->setHtml(QString());

	setFrameShape(QFrame::StyledPanel);

	#ifndef Q_WS_WIN
		setFrameShadow(QFrame::Plain);
	#else
		setFrameShadow(QFrame::Sunken);
	#endif
}
//----------------------------------------------------------------------------------------------

AWebViewWidget::~AWebViewWidget ()
{
	delete View;
}
//----------------------------------------------------------------------------------------------

void AWebViewWidget::resizeEvent (QResizeEvent* event)
{
	View->resize(width() - 4, height() - 4);
	QFrame::resizeEvent(event);
}
//----------------------------------------------------------------------------------------------
