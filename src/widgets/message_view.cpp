#include "message_view.h"
//----------------------------------------------------------------------------------------------
#include "formatter.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

AMessageView::AMessageView (QWidget* parent) : AWebViewWidget (parent), IMessageView ()
{
	m_main_form    = 0;
	m_forum_tree   = 0;
	m_message_tree = 0;

	connect(View->page(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(link_clicked(const QUrl&)));
	connect(View->page(), SIGNAL(linkHovered(const QString&, const QString&, const QString&)), this, SLOT(link_hover(const QString&, const QString&, const QString&)));
}
//----------------------------------------------------------------------------------------------

AMessageView::~AMessageView ()
{
}
//----------------------------------------------------------------------------------------------

void AMessageView::setMainForm (IFormMain* itf)
{
	m_main_form = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageView::setForumTree (IForumTree* itf)
{
	m_forum_tree = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageView::setMessageTree (IMessageTree* itf)
{
	m_message_tree = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageView::setMessage (const AMessageInfo& message, bool special, bool rated)
{
	QString body;

	// загрузка рейтингов (если требуется)
	if (rated)
	{
		// "тихий" контроль ошибок, т.к. не страшно, если пузомерка не загрузится
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() != NULL)
		{
			AMessageRatingList list;

			if (storage->getMessageRatingList(message.ID, list) != false)
				body = AFormatter::formatMessage(message, special, rated, (list.count() > 0 ? &list : NULL));
			else
				body = AFormatter::formatMessage(message, special, rated);
		}
		else
			body = AFormatter::formatMessage(message, special, rated);
	}
	else
		body = AFormatter::formatMessage(message, special, rated);

	View->setHtml(body);
}
//----------------------------------------------------------------------------------------------

void AMessageView::clear ()
{
	View->setHtml(QString());
}
//----------------------------------------------------------------------------------------------

void AMessageView::link_clicked (const QUrl& url)
{
	QString special = url.toString();

	if (m_message_tree != NULL)
		m_message_tree->processUrl(special);
	else
		QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void AMessageView::link_hover (const QString& link, const QString& /*title*/, const QString& /*textContent*/)
{
	bool avalon_link = link.indexOf("avalon:") == 0;

	if (link.length() != 0 && avalon_link == false)
		View->LinkHovered = true;
	else
		View->LinkHovered = false;

	if (avalon_link == false)
		m_main_form->showStatus(link);
}
//----------------------------------------------------------------------------------------------
