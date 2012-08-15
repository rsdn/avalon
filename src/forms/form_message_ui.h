/*!
 * \file
 * \brief GUI формы создания или ответа на сообщение.
 */

#ifndef _avalon_form_message_ui_h_
#define _avalon_form_message_ui_h_

#include "../widgets/web_view.h"
#include "../widgets/spell_edit.h"

/*!
 * \brief GUI формы создания или ответа на сообщение.
 */
class FormMessageUI : public QDialog
{
	Q_OBJECT

	public:

		FormMessageUI  (QWidget* parent);
		~FormMessageUI ();

	protected:

		/*!
		 * \brief Функция сохранения расположения и размеров элементов управления формы <при выходе>
		 */
		void save ();

		/*!
		 * \brief Функция восстановления расположения и размеров элементов управления формы <при старте>
		 */
		void restore ();

		QVBoxLayout* m_layout;                      /*!< \brief Общий layout                             */
		   QToolBar* m_toolbar_message;             /*!< \brief Тулбар                                   */
		      QAction*  m_button_send;              /*!< \brief Кнопка "отправить"                       */
		      QAction*  m_button_draft;             /*!< \brief Кнопка "в черновики"                     */
		      QAction*  m_button_smile;             /*!< \brief Кнопка "смайл"                           */
		         QMenu* m_menu_smile;               /*!< \brief Выпадающее меню со списком улыбок        */
		            QAction* m_menu_smile_lol;      /*!< \brief Меню ":)))"                              */
		            QAction* m_menu_smile_biggrin;  /*!< \brief Меню ":))"                               */
		            QAction* m_menu_smile_smile;    /*!< \brief Меню ":)"                                */
		            QAction* m_menu_smile_frown;    /*!< \brief Меню ":("                                */
		            QAction* m_menu_smile_wink;     /*!< \brief Меню ";)"                                */
		            QAction* m_menu_smile_smirk;    /*!< \brief Меню ":-\\"                              */
		            QAction* m_menu_smile_confused; /*!< \brief Меню ":???:"                             */
		            QAction* m_menu_smile_no;       /*!< \brief Меню ":no:"                              */
		            QAction* m_menu_smile_sup;      /*!< \brief Меню ":up:"                              */
		            QAction* m_menu_smile_down;     /*!< \brief Меню ":down:"                            */
		            QAction* m_menu_smile_super;    /*!< \brief Меню ":super:"                           */
		            QAction* m_menu_smile_shuffle;  /*!< \brief Меню ":shuffle:"                         */
		            QAction* m_menu_smile_wow;      /*!< \brief Меню ":wow:"                             */
		            QAction* m_menu_smile_crash;    /*!< \brief Меню ":crash:"                           */
		            QAction* m_menu_smile_user;     /*!< \brief Меню ":user:"                            */
		            QAction* m_menu_smile_maniac;   /*!< \brief Меню ":maniac:"                          */
		            QAction* m_menu_smile_xz;       /*!< \brief Меню ":xz:"                              */
		            QAction* m_menu_smile_beer;     /*!< \brief Меню ":beer:"                            */
		      QAction*  m_button_bold;              /*!< \brief Кнопка "жирный"                          */
		      QAction*  m_button_italic;            /*!< \brief Кнопка "курсив"                          */
		      QAction*  m_button_quote;             /*!< \brief Кнопка "цитирование"                     */
		      QAction*  m_button_image;             /*!< \brief Кнопка "рисунок"                         */
		      QAction*  m_button_hyperlink;         /*!< \brief Кнопка "гиперссылка"                     */
		      QAction*  m_button_code;              /*!< \brief Кнопка "код"                             */
		         QMenu* m_menu_code;                /*!< \brief Выпадающее меню со списком языков        */
		            QAction* m_menu_code_code;      /*!< \brief Меню "[code]"                            */
		            QAction* m_menu_code_asm;       /*!< \brief Меню "[asm]"                             */
		            QAction* m_menu_code_ccode;     /*!< \brief Меню "[ccode]"                           */
		            QAction* m_menu_code_c;         /*!< \brief Меню "[c]"                               */
		            QAction* m_menu_code_csharp_;   /*!< \brief Меню "[c#]"                              */
		            QAction* m_menu_code_csharp;    /*!< \brief Меню "[csharp]"                          */
		            QAction* m_menu_code_cs;        /*!< \brief Меню "[cs]"                              */
		            QAction* m_menu_code_msil;      /*!< \brief Меню "[msil]"                            */
		            QAction* m_menu_code_midl;      /*!< \brief Меню "[midl]"                            */
		            QAction* m_menu_code_pascal;    /*!< \brief Меню "[pascal]"                          */
		            QAction* m_menu_code_vb;        /*!< \brief Меню "[vb]"                              */
		            QAction* m_menu_code_sql;       /*!< \brief Меню "[sql]"                             */
		            QAction* m_menu_code_perl;      /*!< \brief Меню "[perl]"                            */
		            QAction* m_menu_code_php;       /*!< \brief Меню "[php]"                             */
		            QAction* m_menu_code_java;      /*!< \brief Меню "[java]"                            */
		            QAction* m_menu_code_xml;       /*!< \brief Меню "[xml]"                             */
		            QAction* m_menu_code_lisp;      /*!< \brief Меню "[lisp]"                            */
		            QAction* m_menu_code_haskell;   /*!< \brief Меню "[haskell]"                         */
		            // TODO: тэг [msdn] (используется в 0.06% сообщений)
		            // TODO: тэги [list], [list=1], [list=a], [*] (используются в 0.09% сообщений)
		            // TODO: тэг [hr] (используется в 1.74%)
		            // TODO: [h1] ... [h6]
		            // TODO: [t][th][tr][td] - таблицы
		      QAction*  m_button_char;              /*!< \brief Кнопка "спецсимвол"                      */
		         QMenu* m_menu_char;                /*!< \brief Выпадающее меню со списком спец-символов */
		   QHBoxLayout* m_layout_subject;           /*!< \brief Layout для темы сообщения                */
		      QLabel*    m_label_subject;           /*!< \brief Тема                                     */
		      QLineEdit* m_text_subject;            /*!< \brief Тема                                     */
		   QTabWidget* m_tab_message;               /*!< \brief Таб                                      */
		      QWidget* m_tab_source;                /*!< \brief Таб-панель редактора                     */
		         QHBoxLayout* m_layout_source;      /*!< \brief Layout для редактора сообщения           */
		            ASpellTextEdit* m_text_source;  /*!< \brief Исходный текст сообщения                 */
		      QWidget* m_tab_view;                  /*!< \brief Таб-панель отображения                   */
		         QHBoxLayout* m_layout_view;        /*!< \brief Layout для отображения сообщения         */
		            AWebViewWidget* m_text_view;    /*!< \brief Предпросмотр сообщения                   */

	private slots:

		/*!
		 * \brief Нажатие на кнопку для вставки смайла
		 */
		void button_smile_triggered ();

		void menu_smile_lol_triggered      (); /*!< \brief Меню ":)))"      */
		void menu_smile_biggrin_triggered  (); /*!< \brief Меню ":))"       */
		void menu_smile_smile_triggered    (); /*!< \brief Меню ":)"        */
		void menu_smile_frown_triggered    (); /*!< \brief Меню ":("        */
		void menu_smile_wink_triggered     (); /*!< \brief Меню ";)"        */
		void menu_smile_smirk_triggered    (); /*!< \brief Меню ":-\\"      */
		void menu_smile_confused_triggered (); /*!< \brief Меню ":???:"     */
		void menu_smile_no_triggered       (); /*!< \brief Меню ":no:"      */
		void menu_smile_sup_triggered      (); /*!< \brief Меню ":up:"      */
		void menu_smile_down_triggered     (); /*!< \brief Меню ":down:"    */
		void menu_smile_super_triggered    (); /*!< \brief Меню ":super:"   */
		void menu_smile_shuffle_triggered  (); /*!< \brief Меню ":shuffle:" */
		void menu_smile_wow_triggered      (); /*!< \brief Меню ":wow:"     */
		void menu_smile_crash_triggered    (); /*!< \brief Меню ":crash:"   */
		void menu_smile_user_triggered     (); /*!< \brief Меню ":user:"    */
		void menu_smile_maniac_triggered   (); /*!< \brief Меню ":maniac:"  */
		void menu_smile_xz_triggered       (); /*!< \brief Меню ":xz:"      */
		void menu_smile_beer_triggered     (); /*!< \brief Меню ":beer:"    */

		void button_bold_triggered      (); /*!< \brief Кнопка "жирный"      */
		void button_italic_triggered    (); /*!< \brief Кнопка "курсив"      */
		void button_quote_triggered     (); /*!< \brief Кнопка "цитирование" */
		void button_image_triggered     (); /*!< \brief Кнопка "рисунок"     */
		void button_hyperlink_triggered (); /*!< \brief Кнопка "гиперссылка" */

		/*!
		 * \brief Нажатие на кнопку для вставки тэгов кода
		 */
		void button_code_triggered ();

		void menu_code_code_triggered    (); /*!< \brief Меню "[code]"    */
		void menu_code_asm_triggered     (); /*!< \brief Меню "[asm]"     */
		void menu_code_ccode_triggered   (); /*!< \brief Меню "[ccode]"   */
		void menu_code_c_triggered       (); /*!< \brief Меню "[c]"       */
		void menu_code_csharp__triggered (); /*!< \brief Меню "[c#]"      */
		void menu_code_csharp_triggered  (); /*!< \brief Меню "[csharp]"  */
		void menu_code_cs_triggered      (); /*!< \brief Меню "[cs]"      */
		void menu_code_msil_triggered    (); /*!< \brief Меню "[msil]"    */
		void menu_code_midl_triggered    (); /*!< \brief Меню "[midl]"    */
		void menu_code_pascal_triggered  (); /*!< \brief Меню "[pascal]"  */
		void menu_code_vb_triggered      (); /*!< \brief Меню "[vb]"      */
		void menu_code_sql_triggered     (); /*!< \brief Меню "[sql]"     */
		void menu_code_perl_triggered    (); /*!< \brief Меню "[perl]"    */
		void menu_code_php_triggered     (); /*!< \brief Меню "[php]"     */
		void menu_code_java_triggered    (); /*!< \brief Меню "[java]"    */
		void menu_code_xml_triggered     (); /*!< \brief Меню "[xml]"     */
		void menu_code_lisp_triggered    (); /*!< \brief Меню "[lisp]"    */
		void menu_code_haskell_triggered (); /*!< \brief Меню "[haskell]" */

		/*!
		 * \brief Нажатие на кнопку для вставки спецсимволов
		 */
		void button_char_triggered ();

		/*!
		 * \brief Выбор пункта меню из спецсимволов
		 */
		void menu_char_triggered (QAction* action);
};

#endif   // _avalon_form_message_ui_h_
