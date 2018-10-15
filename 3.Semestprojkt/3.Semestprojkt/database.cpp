#include "database.h"



database::database()
{
}


database::~database()
{
}

bool database::create()
{
	QSqlDatabase db;
	db.addDatabase("QMYSQL");
	db.setConnectOptions("SSL_KEY=client-key.pem;SSL_CERT=client-cert.pem;SSL_CA=ca-cert.pem;CLIENT_IGNORE_SPACE=1");
	db.setHostName("applecake");
	db.setDatabaseName("test_data");
	db.setUserName("group4");
	db.setPassword("applecake");

	bool ok = db.open();           // opens connection for use
	if (ok == false) {
		return false;
	}
	return true;
}
