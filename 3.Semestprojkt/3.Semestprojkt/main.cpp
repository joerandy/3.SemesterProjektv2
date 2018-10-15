#include <stdio.h>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>

#include "communication.h"
#include "database.h"
#include "image.h"


using namespace std;
using namespace cv;

int main() {
	
	communication com;
	com.createSoc();
	

	getchar();
	return 0;
}