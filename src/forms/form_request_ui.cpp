#include "form_request_ui.h"
//----------------------------------------------------------------------------------------------

FormRequestUI::FormRequestUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	setWindowTitle(QString::fromUtf8("HTTP"));

	QIcon icon;
	icon.addFile(":/icons/synchronize16.png",  QSize(16,  16));
	icon.addFile(":/icons/synchronize24.png",  QSize(24,  24));
	icon.addFile(":/icons/synchronize32.png",  QSize(32,  32));
	icon.addFile(":/icons/synchronize48.png",  QSize(48,  48));
	icon.addFile(":/icons/synchronize256.png", QSize(256, 256));
	setWindowIcon(icon);

	resize(400, 300);
	setFixedSize(width(), height());

	m_layout = new QVBoxLayout(this);

	m_layout_form = new QVBoxLayout();
	m_layout->addLayout(m_layout_form);

	m_progress_bar = new QProgressBar(this);
	m_progress_bar->setValue(0);
	m_progress_bar->setMinimum(0);
	m_progress_bar->setMaximum(100);
	m_layout_form->addWidget(m_progress_bar);

	m_list_progress = new QListWidget(this);
	m_list_progress->setFocusPolicy(Qt::NoFocus);

	m_layout_form->addWidget(m_list_progress);

	m_layout_button = new QHBoxLayout();
	m_layout->addLayout(m_layout_button);

	m_layout_button->addStretch();

	m_button_cancel = new QPushButton(this);
	m_button_cancel->setText(QString::fromUtf8("Отмена"));
	m_button_cancel->setShortcut(QKeySequence("Esc"));
	m_layout_button->addWidget(m_button_cancel);

	m_layout_button->addStretch();
}
//----------------------------------------------------------------------------------------------

FormRequestUI::~FormRequestUI ()
{
	delete m_layout_button;
	delete m_layout_form;
}
//----------------------------------------------------------------------------------------------
