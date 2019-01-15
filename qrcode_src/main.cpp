#include <fstream>
#include <iostream>
#include <string>
#include "qr.h"

using namespace std;

int main(int argc, char* argv[]) {
	string blkRst;
	int errcode = QR_ERR_NONE;
	/* argv[1]为mac地址，argv[2]为拍摄时间 */	
	blkRst.append("http://www.sup-heal.com:9080/picture/FiveimageUpload?");
	blkRst.append("picture=").append(argv[1]);
	blkRst.append("&picture1=").append(argv[2]);
	blkRst.append("&picture2=").append(argv[3]);
	blkRst.append("&picture3=").append(argv[4]);
	blkRst.append("&picture4=").append(argv[5]);
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
