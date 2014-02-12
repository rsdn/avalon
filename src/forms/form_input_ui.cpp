#include "form_input_ui.h"
//----------------------------------------------------------------------------------------------

FormInputUI::FormInputUI (QWidget* parent, const QString& header, const QString& message, const QString& text) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	resize(450, 78);
	setFixedSize(width(), height());

	setWindowTitle(header);

	m_layout = new QHBoxLayout(this);

	m_form_layout = new QVBoxLayout();

	m_label = new QLabel(this);
	m_label->setText(message);
	m_label->setMinimumSize(350, 0);
	m_form_layout->addWidget(m_label);

	m_text = new QLineEdit(this);
	m_text->setText(text);
	m_form_layout->addWidget(m_text);

	m_button_layout = new QVBoxLayout();

	m_button_ok = new QPushButton(this);
	m_button_ok->setText(QString::fromUtf8("OK"));
	m_button_ok->setShortcut(QKeySequence("Return"));
	m_button_ok->setDefault(true);
	m_button_layout->addWidget(m_button_ok);

	m_button_cancel = new QPushButton(this);
	m_button_cancel->setText(QString::fromUtf8("Отмена"));
	m_button_cancel->setShortcut(QKeySequence("Esc"));
	m_button_layout->addWidget(m_button_cancel);

	m_spacer_button = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_button_layout->addItem(m_spacer_button);

	m_layout->addLayout(m_form_layout);
	m_layout->addLayout(m_button_layout);

	connect(m_button_ok,     SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));

	m_text->setFocus();
}
//----------------------------------------------------------------------------------------------

FormInputUI::~FormInputUI ()
{
}
//----------------------------------------------------------------------------------------------
