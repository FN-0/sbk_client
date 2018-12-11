#include <fstream>
#include <iostream>
#include <string>
#include "qr.h"

using namespace std;

int main(int argc, char* argv[]) {
	string blkRst;
	int errcode = QR_ERR_NONE;
	/* argv[1]为mac地址，argv[2]为拍摄时间 */
	blkRst.append(argv[1]).append(" ").append(argv[2]);
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
