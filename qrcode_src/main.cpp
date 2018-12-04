#include <fstream>
#include <iostream>
#include <string>
#include "qr.h"

using namespace std;

int main(int argc, char* argv[]) {
	string blkRst = "https://mirrors.shu.edu.cn";
	int errcode = QR_ERR_NONE;
	QRCode* p = qrInit(10, QR_EM_8BIT, 2, -1, &errcode);
	if (p == NULL) {
		printf("error\n");
	}
	qrAddData(p, (const qr_byte_t*)blkRst.data(), blkRst.length());
	if (!qrFinalize(p)) {
		printf("finalize error\n");
	}
	int size = 0;
	qr_byte_t * buffer = qrSymbolToBMP(p, 5, 5, &size);
	if (buffer == NULL) {
		printf("error %s", qrGetErrorInfo(p));
	}
	ofstream f("./qr.bmp");
	f.write((const char *)buffer, size);
	f.close();
	return 0;
}
