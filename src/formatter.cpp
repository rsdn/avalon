#include "formatter.h"
#include "global.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Регексп для проверки URL
 * http://web.archive.org/web/20070807114147/http://www.foad.org/~abigail/Perl/url2.html
 * http://web.archive.org/web/20070705044149/www.foad.org/~abigail/Perl/url3.regex
 */
const QString g_url_regex = "(?:http://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*))*)(?:\\?(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*))?)?)|(?:ftp://(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*)(?::(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*))?@)?(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))(?:/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*))*)(?:;type=[AIDaid])?)?)|(?:news:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;/?:&=])+@(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3})))|(?:[a-zA-Z](?:[a-zA-Z\\d]|[_.+-])*)|\\*))|(?:nntp://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)/(?:[a-zA-Z](?:[a-zA-Z\\d]|[_.+-])*)(?:/(?:\\d+))?)|(?:telnet://(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*)(?::(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*))?@)?(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))/?)|(?:gopher://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))*)(?:%09(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*)(?:%09(?:(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))*))?)?)?)?)|(?:wais://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)/(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)(?:(?:/(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)/(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*))|\\?(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*))?)|(?:mailto:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))+))|(?:file://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))|localhost)?/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*))*))|(?:prospero://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*))*)(?:(?:;(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&])*)=(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&])*)))*)|(?:ldap://(?:(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))?/(?:(?:(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*))(?:(?:(?:%0[Aa])?(?:%20)*)\\+(?:(?:%0[Aa])?(?:%20)*)(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)))*)(?:(?:(?:(?:%0[Aa])?(?:%20)*)(?:[;,])(?:(?:%0[Aa])?(?:%20)*))(?:(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*))(?:(?:(?:%0[Aa])?(?:%20)*)\\+(?:(?:%0[Aa])?(?:%20)*)(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)))*))*(?:(?:(?:%0[Aa])?(?:%20)*)(?:[;,])(?:(?:%0[Aa])?(?:%20)*))?)(?:\\?(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+)(?:,(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))*)?)(?:\\?(?:base|one|sub)(?:\\?(?:((?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))+)))?)?)?)|(?:(?:z39\\.50[rs])://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+)(?:\\+(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))*(?:\\?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))?)?(?:;esn=(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))?(?:;rs=(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+)(?:\\+(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))*)?))|(?:cid:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?:@&=])*))|(?:mid:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?:@&=])*))?)|(?:vemmi://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[/?:@&=])*)(?:(?:;(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[/?:@&])*)=(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[/?:@&])*))*))?)|(?:imap://(?:(?:(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+)(?:(?:;[Aa][Uu][Tt][Hh]=(?:\\*|(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+))))?)|(?:(?:;[Aa][Uu][Tt][Hh]=(?:\\*|(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+)))(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+))?))@)?(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))/(?:(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)?;[Tt][Yy][Pp][Ee]=(?:[Ll](?:[Ii][Ss][Tt]|[Ss][Uu][Bb])))|(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)(?:\\?(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+))?(?:(?:;[Uu][Ii][Dd][Vv][Aa][Ll][Ii][Dd][Ii][Tt][Yy]=(?:[1-9]\\d*)))?)|(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)(?:(?:;[Uu][Ii][Dd][Vv][Aa][Ll][Ii][Dd][Ii][Tt][Yy]=(?:[1-9]\\d*)))?(?:/;[Uu][Ii][Dd]=(?:[1-9]\\d*))(?:(?:/;[Ss][Ee][Cc][Tt][Ii][Oo][Nn]=(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)))?)))?)|(?:nfs:(?:(?://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:(?:/(?:(?:(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*)(?:/(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*))*)?)))?)|(?:/(?:(?:(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*)(?:/(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*))*)?))|(?:(?:(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*)(?:/(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*))*)?)))";

//----------------------------------------------------------------------------------------------
/*!
 * \brief Список простых тэгов для парсера сообщений
 */
ASimpleTag g_simple_tags [] =
{
	{"[b]",    "<b>" },
	{"[/b]",   "</b>"},

	{"[i]",    "<i>" },
	{"[/i]",   "</i>"},

	// upd 22.01.2010, http://www.rsdn.ru/forum/rsdn/3677582.1.aspx
	{"[u]",    "<u>" },
	{"[/u]",   "</u>"},

	// upd 22.01.2010, http://www.rsdn.ru/forum/rsdn/3677582.1.aspx
	{"[s]",    "<strike>" },
	{"[/s]",   "</strike>"},

	// upd 22.01.2010, http://www.rsdn.ru/forum/rsdn/3677582.1.aspx
	{"[cut]",  "<span><a href='#' title='развернуть' onclick=\"obj = this.parentNode.childNodes[1].style; tmp = (obj.display != 'block') ? 'block' : 'none'; obj.display = tmp; return false;\">(развернуть)</a><span style='display: none'><table style='background-color: #FFFFE0;' cellpadding='0' cellspacing='0' width='98%' align='center'><tr><td>" },
	{"[/cut]", "</td></tr></table></span></span>"},

	{"[tr]",   "<tr>" },
	{"[/tr]",  "</tr>"},

	{"[td]",   "<td>" },
	{"[/td]",  "</td>"},

	{"[th]",   "<td align='center' style='background-color: #DCDCDC;'>"},
	{"[/th]",  "</td>"},

	/*{"[img]",  "<img src='"},
	{"[/img]", "'>"},*/

	{"[hr]",   "<hr>"},

	{"[h1]",   "<h1>" },
	{"[/h1]",  "</h1>"},
	{"[h2]",   "<h2>" },
	{"[/h2]",  "</h2>"},
	{"[h3]",   "<h3>" },
	{"[/h3]",  "</h3>"},
	{"[h4]",   "<h4>" },
	{"[/h4]",  "</h4>"},
	{"[h5]",   "<h5>" },
	{"[/h5]",  "</h5>"},
	{"[h6]",   "<h6>" },
	{"[/h6]",  "</h6>"},

	{"[*]",    "<li />"}
};

//----------------------------------------------------------------------------------------------
/*!
 * \brief Список смайлов для парсера сообщений
 */
ASimpleTag g_smile_tags [] =
{
	{":)))",         "<img src='qrc:/smiles/sideways.png'>"  },
	{":))",          "<img src='qrc:/smiles/biggrin.png'>"   },
	{":)",           "<img src='qrc:/smiles/smile.png'>"     },
	{":(",           "<img src='qrc:/smiles/sad.png'>"       },
	{";)",           "<img src='qrc:/smiles/wink.png'>"      },
	{":-\\",         "<img src='qrc:/smiles/pouty.png'>"     },
	{":???:",        "<img src='qrc:/smiles/wacko.png'>"     },
	{":no:",         "<img src='qrc:/smiles/stop.png'>"      },
	{":up:",         "<img src='qrc:/smiles/thumbs_up.png'>" },
	{":down:",       "<img src='qrc:/smiles/thumb_down.png'>"},
	{":super:",      "<img src='qrc:/smiles/cool.png'>"      },
	{":shuffle:",    "<img src='qrc:/smiles/blush.png'>"     },
	{":wow:",        "<img src='qrc:/smiles/shock.png'>"     },
	{":crash:",      "<img src='qrc:/smiles/angry.png'>"     },
	{":user:",       "<img src='qrc:/smiles/music.png'>"     },
	{":maniac:",     "<img src='qrc:/smiles/devil.png'>"     },
	{":xz:",         "<img src='qrc:/smiles/wrong.png'>"     },
	{":beer:",       "<img src='qrc:/smiles/beer.png'>"      }
};

//----------------------------------------------------------------------------------------------
/*!
 * \brief Описатель "монолитного" тэга для парсера сообщений
 */
typedef struct AStrongTag
{
	const char* OpenTag;  /*!< \brief Открывающий тэг */
	const char* CloseTag; /*!< \brief Закрывающий тэг */
} AStrongTag;

//----------------------------------------------------------------------------------------------
/*!
 * \brief Список "монолитных" тэгов для парсера сообщений
 */
AStrongTag g_strong_tags [] =
{
	{"[code]",      "[/code]"     },
	{"[asm]",       "[/asm]"      },
	{"[ccode]",     "[/ccode]"    },
	{"[c]",         "[/c]"        },
	{"[cpp]",       "[/cpp]"      },
	{"[vc]",        "[/vc]"       },
	{"[c#]",        "[/c#]"       },
	{"[csharp]",    "[/csharp]"   },
	{"[cs]",        "[/cs]"       },
	{"[nemerle]",   "[/nemerle]"  },
	{"[msil]",      "[/msil]"     },
	{"[midl]",      "[/midl]"     },
	{"[pascal]",    "[/pascal]"   },
	{"[vb]",        "[/vb]"       },
	{"[sql]",       "[/sql]"      },
	{"[perl]",      "[/perl]"     },
	{"[php]",       "[/php]"      },
	{"[java]",      "[/java]"     },
	{"[xml]",       "[/xml]"      },
	{"[lisp]",      "[/lisp]"     },
	{"[haskell]",   "[/haskell]"  },
	{"[ruby]",      "[/ruby]"     },
	{"[tagline]",   "[/tagline]"  },
	{"[moderator]", "[/moderator]"},
	{"[q]",         "[/q]"        },
	{"[t]",         "[/t]"        },

	// avalon specific, http://rsdn.ru/forum/rsdn/2240671.1.aspx
	{"[graphviz]",  "[/graphviz]" },

	// http://www.rsdn.ru/forum/message/3227340.1.aspx
	{"[code=]",            "[/code]"},
	{"[code=assembler]",   "[/code]"},
	{"[code=c]",           "[/code]"},
	{"[code=csharp]",      "[/code]"},
	{"[code=cs]",          "[/code]"},
	{"[code=nemerle]",     "[/code]"},
	{"[code=erlang]",      "[/code]"},
	{"[code=haskell]",     "[/code]"},
	{"[code=idl]",         "[/code]"},
	{"[code=java]",        "[/code]"},
	{"[code=lisp]",        "[/code]"},
	{"[code=msil]",        "[/code]"},
	{"[code=ocaml]",       "[/code]"},
	{"[code=pascal]",      "[/code]"},
	{"[code=perl]",        "[/code]"},
	{"[code=php]",         "[/code]"},
	{"[code=prolog]",      "[/code]"},
	{"[code=python]",      "[/code]"},
	{"[code=ruby]",        "[/code]"},
	{"[code=sql]",         "[/code]"},
	{"[code=visualbasic]", "[/code]"},
	{"[code=xsl]",         "[/code]"},
	// http://www.rsdn.ru/forum/cpp/3482377.1.aspx
	{"[code=cpp]",         "[/code]"}
};

//----------------------------------------------------------------------------------------------
/*!
 * \brief Блок текста для парсера исходного текста сообщения и формирования его HTML отображения
 */
typedef struct AMessageBlock
{
	QString Tag;   /*!< \brief Содержимое тэга вместе со скобками */
	QString Body;  /*!< \brief Тело тэга                          */
} AMessageBlock;
//----------------------------------------------------------------------------------------------
/*!
 * \brief Список замены для подсветки синтаксиса Highlight.js
 * см. http://softwaremaniacs.org/soft/highlight/
 */
ASimpleTag g_highlight_tags [] =
{
	{"[code]",    ""          },
	{"[asm]",     "avrasm"    },
	{"[ccode]",   "cpp"       },
	{"[c]",       "cpp"       },
	{"[cpp]",     "cpp"       }, // avalon specific, в FAQ нету такого, но, как выяснилось, встречается
	{"[vc]",      "cpp"       }, // avalon specific, в FAQ нету такого, но, как выяснилось, встречается
	{"[c#]",      "cs"        },
	{"[csharp]",  "cs"        },
	{"[cs]",      "cs"        },
	{"[nemerle]", "cs"        }, // ?
	{"[msil]",    "cpp"       }, // ?
	{"[midl]",    "cpp"       }, // ?
	{"[pascal]",  "delphi"    },
	{"[vb]",      "vbscript"  }, // ?
	{"[sql]",     "sql"       },
	{"[perl]",    "perl"      },
	{"[php]",     "php"       },
	{"[java]",    "java"      },
	{"[js]",      "javascript"}, // avalon specific
	{"[xml]",     "xml"       },
	{"[lisp]",    "lisp"      },
	{"[haskell]", "haskell"   },
	{"[ruby]",    "ruby"      },

	// http://www.rsdn.ru/forum/message/3227340.1.aspx
	{"[code=]",            ""        },
	{"[code=assembler]",   "avrasm"  },
	{"[code=c]",           "cpp"     },
	{"[code=cpp]",         "cpp"     },
	{"[code=csharp]",      "cs"      },
	{"[code=cs]",          "cs"      },
	{"[code=nemerle]",     "cs"      },
	{"[code=erlang]",      "erlang"  },
	{"[code=haskell]",     "haskell" },
	{"[code=idl]",         "cpp"     }, // ?
	{"[code=java]",        "java"    },
	{"[code=lisp]",        "lisp"    },
	{"[code=msil]",        "cpp"     }, // ?
	{"[code=ocaml]",       "lisp"    }, // ?
	{"[code=pascal]",      "delphi"  },
	{"[code=perl]",        "perl"    },
	{"[code=php]",         "php"     },
	{"[code=prolog]",      "lisp"    }, // ?
	{"[code=python]",      "python"  },
	{"[code=ruby]",        "ruby"    },
	{"[code=sql]",         "sql"     },
	{"[code=visualbasic]", "vbscript"},
	{"[code=xsl]",         "xml"     },
	// http://www.rsdn.ru/forum/cpp/3482377.1.aspx
	{"[code=cpp]",         "cpp"     },

	{NULL, NULL}
};
//----------------------------------------------------------------------------------------------

QString AFormatter::formatMessage (const AMessageInfo& message, bool special, bool rated, const AMessageRatingList* rating_list)
{
	AGlobal::getInstance()->clearTempList();

	QString data = message.Message;

	QList<AMessageBlock*> list;

	AMessageBlock* block = new AMessageBlock();

	list.append(block);

	while (data.length() > 0)
	{
		// поиск начала тэга
		int pos1 = data.indexOf("[");

		if (pos1 == -1)
		{
			block->Body += data;
			break;
		}

		if (pos1 + 1 == data.length())
		{
			block->Body += data;
			break;
		}

		// проверка на "[["
		if (data[pos1 + 1] == '[')
		{
			block->Body += data.mid(0, pos1) + "&#x5B;";
			data.remove(0, pos1 + 2);
			continue;
		}

		// найдено открытие начала тэга, поиск закрытия
		int pos2 = data.indexOf("]", pos1);

		if (pos2 == -1)
		{
			block->Body += data;
			break;
		}

		// получение тэга
		QString tag = data.mid(pos1, pos2 - pos1 + 1).toLower();

		// игнорируем простые тэги
		AStrongTag* found_tag = NULL;

		for (size_t i = 0; i < sizeof(g_strong_tags) / sizeof(AStrongTag); i++)
			if (tag == g_strong_tags[i].OpenTag)
			{
				found_tag = &g_strong_tags[i];
				break;
			}

		if (found_tag == NULL)
		{
			block->Body += data.mid(0, pos2 + 1);
			data.remove(0, pos2 + 1);
			continue;
		}

		// найден не простой тэг
		QString close_tag = found_tag->CloseTag;

		int pos3 = data.indexOf(close_tag, pos2 + 1, Qt::CaseInsensitive);

		if (pos3 == -1)
		{
			block->Body += data.mid(0, pos2 + 1);
			data.remove(0, pos2 + 1);
			continue;
		}

		// найдено закрытие непростого тэга

		// копируем хвост в текущий блок
		block->Body += data.mid(0, pos1);

		// создание нового блока для спец-тэга
		block = new AMessageBlock();

		list.append(block);

		block->Tag  = tag;
		block->Body = data.mid(pos2 + 1, pos3 - pos2 - 1);

		data.remove(0, pos3 + close_tag.length());

		// открываем новый блок
		block = new AMessageBlock();

		list.append(block);
	}

	QString	result;

	result += "<html>";
	result += "<head>";
	result += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";

	// подключение highlight.js
	QString path = QCoreApplication::applicationDirPath();

	#if QT_VERSION >= 0x040500
	result += "<style type='text/css'>pre { font-family: 'Courier New', courier; font-size: 11pt; }</style>";
	#endif

	#ifdef Q_WS_WIN
		result += "<link rel='stylesheet' title='Magula' href='file:///" + path + "/highlight/styles/magula.css'>";
		result += "<script src='file:///" + path + "/highlight/highlight.pack.js'></script>";
	#else
		result += "<link rel='stylesheet' title='Magula' href='file://" + path + "/highlight/styles/magula.css'>";
		result += "<script src='file://" + path + "/highlight/highlight.pack.js'></script>";
	#endif

	result += "<script>hljs.initHighlightingOnLoad();</script>";

	result += "<style type='text/css'>table { font-size: 11pt; }</style>";
	result += "</head>";
	result += "<body topmargin='0' leftmargin='0' rightmargin='0' bottommargin='0'>";
	result += "<table width='100%' cellpadding='1' cellspacing='0' border='0'>";

	result += "<tr style='background-color: #E6F2E6' valign='center'>";

	QString subject = message.Subject;

	subject.replace("&", "&amp;");
	subject.replace("<", "&lt;");
	subject.replace(">", "&gt;");

	result += "<td><a href='http://www.rsdn.ru/forum/message/" + QString::number(message.ID) + ".aspx'>" + QString::fromUtf8("<img src='qrc:/icons/show_topic.png' title='показать положение в теме' alt='показать положение в теме'>") + "</a><b>&nbsp;";

	if (message.ID != 0 && special == false)
		result += "<a style='color: black; text-decoration: none;' href='http://www.rsdn.ru/forum/message/" + QString::number(message.ID) + ".1.aspx'>" + subject + "</a>";
	else
		result += subject;

	result += "</b></td>";

	result += "<td align='right'>";

	if (rated == true)
	{
		result += "<a href='avalon:rate_plus_1'><img src='qrc:/icons/rate_plus_1.png' title='+1' alt='+1'></a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_1'>" + QString::fromUtf8("<img src='qrc:/icons/rate_1.png' title='интересно' alt='интересно'>") + "</a>";
		result += "<a href='avalon:rate_2'>" + QString::fromUtf8("<img src='qrc:/icons/rate_2.png' title='спасибо' alt='спасибо'>") + "</a>";
		result += "<a href='avalon:rate_3'>" + QString::fromUtf8("<img src='qrc:/icons/rate_3.png' title='супер' alt='супер'>") + "</a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_cross'>" + QString::fromUtf8("<img src='qrc:/icons/rate_cross.png' title='удалить оценку' alt='удалить оценку'>") + "</a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_smile'>" + QString::fromUtf8("<img src='qrc:/icons/rate_smile.png' title='смешно' alt='смешно'>") + "</a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_plus'>" + QString::fromUtf8("<img src='qrc:/icons/rate_plus.png' title='согласен' alt='согласен'>") + "</a>";
		result += "<a href='avalon:rate_minus'>" + QString::fromUtf8("<img src='qrc:/icons/rate_minus.png' title='не согласен' alt='не согласен'>") + "</a>";
		result += "&nbsp;&nbsp;";
	}

	if (special == false)
		result += "<a href='avalon:moderate'>" + QString::fromUtf8("<img src='qrc:/icons/moderate.png' title='модерирование' alt='модерирование'>") + "</a>";

	result += "&nbsp;&nbsp;";
	result += "</td></tr>";

	if (message.IDUser == 0 || message.IDUser == -1)
	{
		if (message.UserNick.length() > 0)
			result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("От:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>" + message.UserNick + "</td></tr>";
		else
			result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("От:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>" + QString::fromUtf8("Аноним") + "</td></tr>";
	}
	else
		result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("От:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><a href='http://www.rsdn.ru/Users/" + QString::number(message.IDUser) + ".aspx'>" + message.UserNick + "</a></td></tr>";

	result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("Дата:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>" + message.MessageDate.toString("dd.MM.yyyy HH:mm") + "</td></tr>";

	// формирование строки рейтинга
	if (rating_list != NULL && rating_list->count() > 0)
	{
		/* "+1" = -3, "1" = 1, "2" = 2, "3" = 3, "+" = -4, "-" = 0, ";)" = -2 */
		int rate_plus_one = 0;
		int rate_one      = 0;
		int rate_two      = 0;
		int rate_three    = 0;
		int rate_plus     = 0;
		int rate_minus    = 0;
		int rate_smile    = 0;

		for (int i = 0; i < rating_list->count(); i++)
		{
			AMessageRating info = rating_list->at(i);

			int rate = info.Rate;

			if (rate == 1)
				rate_one++;
			else if (rate == 2)
				rate_two++;
			else if (rate == 3)
				rate_three++;
			else if (rate == 0)
				rate_minus++;
			else if (rate == -2)
				rate_smile++;
			else if (rate == -3)
				rate_plus_one++;
			else if (rate == -4)
				rate_plus++;
		}

		// может оказаться, что есть всего одна оценка и она не входит в диапазон допустимых - получится пустое пространство
		if (rate_plus_one > 0 || rate_one > 0 || rate_two > 0 || rate_three > 0 || rate_plus > 0 || rate_minus > 0 || rate_smile > 0)
		{
			result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;<a style='color: black; text-decoration: none;' href='http://www.rsdn.ru/forum/RateList.aspx?mid=" + QString::number(message.ID) + QString::fromUtf8("'>Оценки</a>:") + "&nbsp;&nbsp;</b>";

			if (rate_smile > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_smile.png' align='top' title='смешно' alt='смешно'>&nbsp;") + QString::number(rate_smile) + "&nbsp;&nbsp;";
			if (rate_plus > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_plus.png' align='top' title='согласен' alt='согласен'>&nbsp;") + QString::number(rate_plus) + "&nbsp;&nbsp;";
			if (rate_minus > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_minus.png' align='top' title='не согласен' alt='не согласен'>&nbsp;") + QString::number(rate_minus) + "&nbsp;&nbsp;";
			if (rate_plus_one > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_plus_1.png' align='top' title='+1' alt='+1'>&nbsp;") + QString::number(rate_plus_one) + "&nbsp;&nbsp;";
			if (rate_one > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_1.png' align='top' title='интересно' alt='интересно'>&nbsp;") + QString::number(rate_one) + "&nbsp;&nbsp;";
			if (rate_two > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_2.png' align='top' title='спасибо' alt='спасибо'>&nbsp;") + QString::number(rate_two) + "&nbsp;&nbsp;";
			if (rate_three > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_3.png' align='top' title='супер' alt='супер'>&nbsp;") + QString::number(rate_three) + "&nbsp;&nbsp;";

			result += "</td></tr>";
		}
	}

	//if (message.ID != 0 && special == false)
	//	result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;URL:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><a href='http://www.rsdn.ru/forum/message/" + QString::number(message.ID) + ".1.aspx'>http://www.rsdn.ru/forum/message/" + QString::number(message.ID) + ".1.aspx</a></td></tr>";

	result += "<tr><td colspan='2'><table width='99%' align='center'><tr><td><br />";

	int index = 0;

	// удаление пустых блоков
	while (index < list.size())
	{
		AMessageBlock* block = list.at(index);

		if (block->Body.trimmed().length() == 0)
		{
			delete block;
			list.removeAt(index);
			continue;
		}

		index++;
	}

	// формирование результата
	int size = list.size();

	for (int i = 0; i < size; i++)
	{
		AMessageBlock* block = list.at(0);

		if (block->Body.trimmed().length() == 0)
		{
			delete block;
			list.removeAt(0);
			continue;
		}

		if (block->Tag == "")
		{
			result += processSimpleText(block->Body.trimmed(), message) + "<br /><br />";
		}

		else if (block->Tag == "[q]")
		{
			result += "<table style='background-color: #FFFFE0;' width='98%' align='center'><tr><td>";
			result += processSimpleText(block->Body.trimmed(), message);
			result += "</td></tr></table><br />";
		}

		else if (block->Tag == "[moderator]")
		{
			result += "<table style='background-color: #FFC0C0;' width='100%'><tr><td>";
			result += processSimpleText(block->Body.trimmed(), message);
			result += "</td></tr></table>";

			// бывает так, что пишут текст после сообщения модератора
			if (i + 1 != size)
				result += "<br />";
		}

		else if (block->Tag == "[tagline]")
		{
			result += "<font color='#A52A2A' size=-1>";
			result += processSimpleText(block->Body.trimmed(), message);
			result += "</font><br />";
		}
		else if (block->Tag == "[t]")
		{
			QString temp = block->Body.trimmed();
			QRegExp r("\\]((\r\n)|(\n))+\\[");
			temp.replace(r, "][");

			result += "<table align='center'>";
			result += processSimpleText(temp, message);
			result += "</table><br />";
		}

		else if (block->Tag == "[graphviz]")
		{
			QTemporaryFile* file = new QTemporaryFile();

			if (file->open() == true)
			{
				QString program = "dot -Tgif -o " + file->fileName();

				QProcess process;

				process.start(program);

				if (process.waitForStarted() == true)
				{
					QString plain_source = block->Body.trimmed();

					plain_source.replace("&lt;",  "<");
					plain_source.replace("&gt;",  ">");
					plain_source.replace("&amp;", "&");

					process.write(plain_source.toUtf8());

					process.closeWriteChannel();

					if (process.waitForFinished() == true)
					{
						AGlobal::getInstance()->TempFileList.append(file);

						#ifdef Q_WS_WIN
						result += "<p align='center'><img src='file:///" + file->fileName() + "'></p>";
						#else
						result += "<p align='center'><img src='file://" + file->fileName() + "'></p>";
						#endif
					}
					else
						delete file;
				}
				else
					delete file;
			}
			else
				delete file;
		}
		else
		{
			// костыль для того, чтобы удалить лишние строки в начале и конце блока
			QStringList lst = block->Body.split("\n");

			while (lst.size() && lst.at(0).trimmed().length() == 0)
				lst.removeAt(0);

			while (lst.size() && lst.at(lst.size() - 1).trimmed().length() == 0)
				lst.removeAt(lst.size() - 1);

			block->Body = lst.join("\n");

			// табуляторы на стандартные(?) 4 пробела
			block->Body.replace("\t", "    ");

			// массив тэгов для подсветки кода
			ASimpleTag* code_tags = g_highlight_tags;

			while (code_tags->Source != NULL /* проход до последнего элемента массива тэгов, заданного {Source => NULL, Replace => NULL} */)
			{
				QString source  = QString::fromUtf8(code_tags->Source);
				QString replace = QString::fromUtf8(code_tags->Replace);

				if (block->Tag == source)
				{
					if (replace.length() == 0)
						break;

					block->Body = "<code class='" + replace + "'>" + block->Body + "</code>";

					break;
				}

				code_tags++;
			}

			result += "<table width='98%' align='center'><tr><td><pre>";
			result += block->Body;
			result += "</pre></tr></td></table><br />";
		}

		delete block;

		list.removeAt(0);
	}

	result += "</td></tr></table></td></tr>";
	result += "<tr style='background-color: #E6F2E6'><td colspan='2'><b>&nbsp;</b></td></tr>";

	result += "</table>";
	result += "</body>";
	result += "</html>";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::processSimpleText (const QString& text, const AMessageInfo& message)
{
	QString data = text;

	// замена стандартных тэгов
	for (size_t i = 0; i < sizeof(g_simple_tags) / sizeof(ASimpleTag); i++)
		data.replace(QString::fromUtf8(g_simple_tags[i].Source), QString::fromUtf8(g_simple_tags[i].Replace), Qt::CaseInsensitive);

	for (size_t i = 0; i < sizeof(g_smile_tags) / sizeof(ASimpleTag); i++)
		data.replace(g_smile_tags[i].Source, g_smile_tags[i].Replace, Qt::CaseInsensitive);

	// выравнивание (или нет) изображений (см. #87)
	QRegExp img1("(^|\r|\n)\\s{0,}\\[img\\]\\s{0,}(\\S+)\\s{0,}\\[/img\\]\\s{0,}(\r|\n|$)", Qt::CaseInsensitive);
	QRegExp img2("\\[img\\]\\s{0,}(\\S+)\\s{0,}\\[/img\\]", Qt::CaseInsensitive);

	img1.setMinimal(true);
	img2.setMinimal(true);

	data.replace(img1, "<p align='center'><img src='\\2'></p>");
	data.replace(img2, "<img src='\\1'>");

	// исправление кривых урлов, которые создаются криворукими топик-стартерами
	// Пример [http://www.rsdn.ru/forum/flame.comp/3673367.1.aspx]
	QRegExp url0(QString::fromUtf8("\\[url=(здесь|тут)\\](\\S+)\\[/url\\]"), Qt::CaseInsensitive);
	url0.setMinimal(true);
	data.replace(url0, QString::fromUtf8("<a href='\\2'>здесь</a>"));

	// стандартные url
	QRegExp url1("\\[url=data:(\\S+)\\](.+)\\[/url\\]", Qt::CaseInsensitive);
	QRegExp url2("\\[url=(\\S+)\\](.+)\\[/url\\]",      Qt::CaseInsensitive);
	QRegExp url3("\\[url\\](\\S+)\\[/url\\]",           Qt::CaseInsensitive);

	url1.setMinimal(true);
	url2.setMinimal(true);
	url3.setMinimal(true);

	data.replace(url1, "<a href='bad link'>\\2</a>");
	data.replace(url2, "<a href='\\1'>\\2</a>");
	data.replace(url3, "<a href='\\1'>\\1</a>");

	// email url
	QRegExp email("\\[email\\](\\S+)\\[/email\\]",  Qt::CaseInsensitive);
	email.setMinimal(true);
	data.replace(email, (QString)"<a href='mailto:\\1?subject=" + QUrl::toPercentEncoding("RE: " + message.Subject).constData() + "'>\\1</a>");

	// msdn url
	QRegExp msdn("\\[msdn\\](\\S+)\\[/msdn\\]",    Qt::CaseInsensitive);
	msdn.setMinimal(true);
	data.replace(msdn, "<a href='http://search.msdn.microsoft.com/Default.aspx?brand=Msdn&query=\\1'>\\1</a>");

	// замена списков
	QRegExp list1("\\[list=1\\](.+)\\[/list\\]", Qt::CaseInsensitive);
	QRegExp list2("\\[list=a\\](.+)\\[/list\\]", Qt::CaseInsensitive);
	QRegExp list3("\\[list\\](.+)\\[/list\\]",   Qt::CaseInsensitive);

	list1.setMinimal(true);
	list2.setMinimal(true);
	list3.setMinimal(true);

	data.replace(list1, "<ol type='1' start='1' style='margin-top:0; margin-bottom:0;'>\\1</ol>");
	data.replace(list2, "<ol type='a' style='margin-top:0; margin-bottom:0;'>\\1</ol>");
	data.replace(list3, "<ul style='margin-top:0; margin-bottom:0;'>\\1</ul>");

	// подстановка тэга cut=text
	QRegExp cut1("\\[cut=(.+)\\]", Qt::CaseInsensitive);

	cut1.setMinimal(true);

	data.replace(cut1, "<span><a href='#' title='\\1' onclick=\"obj = this.parentNode.childNodes[1].style; tmp = (obj.display != 'block') ? 'block' : 'none'; obj.display = tmp; return false;\">\\1</a><span style='display: none'><table style='background-color: #FFFFE0;' cellpadding='0' cellspacing='0' width='98%' align='center'><tr><td>");

	// исправление кривых урлов (для которых не проставлен тэг)
	//
	// универсальный regexp для URL:
	// http://web.archive.org/web/20070807114147/http://www.foad.org/~abigail/Perl/url2.html
	// http://web.archive.org/web/20070705044149/www.foad.org/~abigail/Perl/url3.regex
	//
	// универсальный regexp для e-mail:
	// http://ex-parrot.com/~pdw/Mail-RFC822-Address.html
	//
	// однако, ценность применения тких монстров несколько сомнительна...
	// хотя, если кому-то удастся прикрутить ЭТО...

	QRegExp url5("(^|[^/])(www\\.\\S+)", Qt::CaseInsensitive);
	// TODO: необходимо исправление, см. http://www.rsdn.ru/forum/web/3988233.1.aspx
	QRegExp url6("(^|[^'>])((http|https|ftp)://[a-zA-Z0-9\\-_\\./\\?\\&\\=\\%\\+\\:\\;\\#]+[a-zA-Z0-9\\-_/\\?\\&\\=\\%\\+\\:\\;])(\\s|,|\\.[^a-zA-Z0-9\\-_/\\?\\&\\=\\%]|\\[|\\]|\\;|\\)|\\(|$)", Qt::CaseInsensitive);

	data.replace(url5, "\\1http://\\2");
	data.replace(url6, "\\1<a href='\\2'>\\2</a>\\4");

	//
	// исправление кривого квотинга
	//

	QStringList source = data.split("\n");

	// удаление дублирующихся пустых строк
	int index = 0;

	// регексп для приветствий в квотинге
	QRegExp hello(QString::fromUtf8("&gt;\\s*Здравствуйте, .+, Вы писали:"));

	while (index < source.size() - 1)
	{
		if (source.at(index).trimmed().length() == 0 && source.at(index + 1).trimmed().length() == 0)
		{
			source.removeAt(index);
			continue;
		}
		else if (source.at(index).indexOf(hello) != -1)
		{
			source.removeAt(index);

			if (index > 0)
				index--;

			continue;
		}

		index++;
	}

	// вычисление уровня квотинга
	QList<int> quoting_level;

	int size = source.size();

	for (int i = 0; i < size; i++)
	{
		QString temp = source.at(i).trimmed();

		int level = 0;

		for (int j = 0; j < temp.length() - 3; j++)
		{
			// после символа квотинга может быть пробел для красивого оформления
			if (temp[j] == ' ' && !(temp[j + 1] == '&' || temp[j + 2] == '&' || temp[j + 3] == '&'))
				break;
			// встречен символ квотинга ">"
			else if (temp[j] == '&' && temp[j + 1] == 'g' && temp[j + 2] == 't' && temp[j + 3] == ';')
			{
				level++;
				j += 3;
			}
			// встречен обратный знак "<" - после него квотинга точно нет
			else if (temp[j] == '&' && temp[j + 1] == 'l' && temp[j + 2] == 't' && temp[j + 3] == ';')
				break;
		}

		quoting_level.append(level);

		source[i] = temp;
	}

	// вставка пустых строк между разными уровнями квотинга
	for (int i = 0; i < size - 1; i++)
	{
		if (source[i].length() == 0)
			continue;

		if (quoting_level.at(i) != quoting_level.at(i + 1) && source[i + 1].length() /*&& (quoting_level.at(i) == 0 || quoting_level.at(i + 1) == 0)*/)
			source[i] = source[i] + "<br />";
	}

	// подсветка квотинга
	for (int i = 0; i < size; i++)
	{
		if (quoting_level.at(i) != 0)
		{
			if (quoting_level.at(i) % 3 == 0)
				source[i] = "<font color=darkblue>" + source[i] + "</font>";
			else if (quoting_level.at(i) % 2 == 0)
				source[i] = "<font color=darkred>" + source[i] + "</font>";
			else
				source[i] = "<font color=darkgreen>" + source[i] + "</font>";
		}
	}

	data = source.join("<br />");

	return data;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::normalizeBody (const QString& body, const QString& nick)
{
	QString data = body;

	// отбрасываем 	"[tagline]" и "[moderator]"
	QRegExp tagline("\\[tagline\\](.+)\\[/tagline\\]",       Qt::CaseInsensitive);
	QRegExp moderator("\\[moderator\\](.+)\\[/moderator\\]", Qt::CaseInsensitive);

	tagline.setMinimal(true);
	moderator.setMinimal(true);

	data.replace(tagline,   "");
	data.replace(moderator, "");

	// удаляем img с даными вместо ссылки (например, см. http://www.rsdn.ru/forum/flame.comp/4077971.1.aspx)
	QRegExp img1("\\[img\\]data:(\\S+)\\[/img\\]", Qt::CaseInsensitive);
	img1.setMinimal(true);
	data.replace(img1, "");

	// заменяем [img]...[/img] на ссылку
	QRegExp img2("\\[img\\](\\S+)\\[/img\\]", Qt::CaseInsensitive);
	img2.setMinimal(true);
	data.replace(img2, QString::fromUtf8("[url=\\1]\\1[/url]"));

	// укорачивание длинных ссылок (например, см. http://www.rsdn.ru/forum/web/4086359.1.aspx)
	QRegExp url1("\\[url=data:(\\S+)\\](.+)\\[/url\\]", Qt::CaseInsensitive);
	url1.setMinimal(true);
	data.replace(url1, "[url=bad%20link]\\2[/url]");

	data = data.trimmed();

	// разбиение
	QStringList source = data.split("\n");

	QString nick_3 = AFormatter::nick3(nick);

	// добавление квотинга к строкам
	for (int i = 0; i < source.size(); i++)
		if (source[i].trimmed().length())
			source[i] = nick_3 + "> " + source[i];

	// удаление дублирующихся пустых строк
	int index = 0;

	// регексп для приветствий в квотинге
	QRegExp hello(QString::fromUtf8(">\\s{0,}Здравствуйте,\\s.+,\\sВы писали:"));

	while (index < source.size() - 1)
	{
		if (source.at(index).trimmed().length() == 0 && source.at(index + 1).trimmed().length() == 0)
		{
			source.removeAt(index);
			continue;
		}
		else if (source.at(index).indexOf(hello) != -1)
		{
			source.removeAt(index);

			if (index > 0)
				index--;

			continue;
		}

		index++;
	}

	// вычисление уровня квотинга
	QList<int> quoting_level;

	int size = source.size();

	for (int i = 0; i < size; i++)
	{
		QString temp = source.at(i).trimmed();

		int level = 0;

		for (int j = 0; j < temp.length() - 3; j++)
		{
			if (temp[j] == ' ' && !(temp[j + 1] == '&' || temp[j + 2] == '&' || temp[j + 3] == '&'))
				break;
			else if (temp[j] == '&' && temp[j + 1] == 'g' && temp[j + 2] == 't' && temp[j + 3] == ';')
			{
				level++;
				j += 3;
			}
			else if (temp[j] == '&' && temp[j + 1] == 'l' && temp[j + 2] == 't' && temp[j + 3] == ';')
				break;
		}

		quoting_level.append(level);

		source[i] = temp;
	}

	// вставка пустых строк между разными уровнями квотинга
	for (int i = 0; i < size - 1; i++)
	{
		if (source[i].length() == 0)
			continue;

		if (quoting_level.at(i) != quoting_level.at(i + 1) && source[i + 1].length() /*&& (quoting_level.at(i) == 0 || quoting_level.at(i + 1) == 0)*/)
			source[i] = source[i] + "\r\n";
	}

	data = source.join("\r\n").trimmed();

	// замена HTML спец-символов
	data.replace("&gt;",  ">");
	data.replace("&lt;",  "<");
	data.replace("&amp;", "&");

	return data;
}
//----------------------------------------------------------------------------------------------

int AFormatter::isURL (const QString& text)
{
	QRegExp url(g_url_regex, Qt::CaseInsensitive);

	if (url.exactMatch(text) == true)
		return 1;

	if (url.exactMatch("http://" + text) == true)
		return 2;

	return 0;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::nick3 (const QString& nick)
{
	if (nick.length() < 4)
		return nick;

	QString result;

	for (int i = 0; i < nick.length(); i++)
		if (nick[i].isUpper() == true)
			result += nick[i];

	if (result.length() == 0)
		result = nick[0];

	if (result.length() > 3)
		result.resize(3);

	return result;
}
//----------------------------------------------------------------------------------------------
