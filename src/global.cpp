#include "global.h"
//----------------------------------------------------------------------------------------------
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------
AGlobal* g_global = NULL;
//----------------------------------------------------------------------------------------------

AGlobal* AGlobal::getInstance ()
{
	if (g_global != NULL)
		return g_global;

	return new AGlobal();
}
//----------------------------------------------------------------------------------------------

AGlobal::AGlobal ()
{
	g_global = this;

	// предпочитаемый список шифров
	// $ openssl ciphers -tls1 'HIGH:!TLSv1.2:!aNULL:!MD5:!3DES:!CAMELLIA:!SRP:!PSK:@STRENGTH'
	// TODO: подключить TLSv1.2 шифры для Qt >= 5.4
	static const char* AVALON_CIPHERS[] =
	{
		"ECDHE-RSA-AES256-SHA",
		"ECDHE-ECDSA-AES256-SHA",
		"DHE-RSA-AES256-SHA",
		"DHE-DSS-AES256-SHA",
		"ECDH-RSA-AES256-SHA",
		"ECDH-ECDSA-AES256-SHA",
		"AES256-SHA",
		"ECDHE-RSA-AES128-SHA",
		"ECDHE-ECDSA-AES128-SHA",
		"DHE-RSA-AES128-SHA",
		"DHE-DSS-AES128-SHA",
		"ECDH-RSA-AES128-SHA",
		"ECDH-ECDSA-AES128-SHA",
		"AES128-SHA",
		NULL
	};

	// поддерживаемый список шифров
	QList<QSslCipher> cipher_list;

	const char** ciphers = AVALON_CIPHERS;
	while ((*ciphers) != NULL)
	{
		QSslCipher cipher(*ciphers, QSsl::SslV3);
		if (cipher.isNull() == false)
			cipher_list.append(cipher);

		ciphers++;
	}

	// задание конфигурации ssl по умолчанию
	QSslConfiguration ssl_default = QSslConfiguration::defaultConfiguration();

#if QT_VERSION < 0x050000
	// до версии 5.0.x поддерживается только TLSv1
	// т.о. для отключения SSLv3 включаем принудительный TLSv1
	ssl_default.setProtocol(QSsl::TlsV1);
#else
	#if QT_VERSION < 0x050400
		// до версии 5.4.0 для QSsl::SecureProtocols используется
		// SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2
		// т.о. для отключения SSLv3 включаем принудительный TLSv1
		ssl_default.setProtocol(QSsl::TlsV1_0);
	#else
		// в версии 5.4.0 для QSsl::SecureProtocols используется
		// SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3
		// т.о. можем работать TLSv1+
		ssl_default.setProtocol(QSsl::SecureProtocols);
	#endif
#endif

	ssl_default.setCiphers(cipher_list);

	QSslConfiguration::setDefaultConfiguration(ssl_default);

	// значения по умолчанию
	AnonymousName = QString::fromUtf8("Аноним");
	DateFormat    = "dd.MM.yyyy hh:mm:ss";

	Me.ID = 0;

	reload();
}
//----------------------------------------------------------------------------------------------

AGlobal::~AGlobal ()
{
	g_global = NULL;
}
//----------------------------------------------------------------------------------------------

void AGlobal::reload ()
{
	QSettings settings;

#ifdef AVALON_USE_ZLIB
	Compression = settings.value("storage/compression", false).toInt();
#endif

	// информация о текущем пользователе avalon
	Me.ID             = -1;
	Me.Name           = "(n/a)";
	Me.Nick           = "(n/a)";
	Me.RealName       = "(n/a)";
	Me.Email          = "(n/a)";
	Me.Homepage       = "(n/a)";
	Me.Specialization = "(n/a)";
	Me.WhereFrom      = "(n/a)";
	Me.Origin         = "(n/a)";

	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() != NULL)
	{
		Me.Name = settings.value("rsdn/login", "").toString();
		storage->whoAmI(Me);
	}
}
//----------------------------------------------------------------------------------------------
