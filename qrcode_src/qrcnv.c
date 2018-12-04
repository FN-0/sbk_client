/*
 * QR Code Generator Library: Symbol Converters for Basic Output Formats
 *
 * Core routines were originally written by Junn Ohta.
 * Based on qr.c Version 0.1: 2004/4/3 (Public Domain)
 *
 * @package     libqr
 * @author      Ryusuke SEKIYAMA <rsky0711@gmail.com>
 * @copyright   2006-2013 Ryusuke SEKIYAMA
 * @license     http://www.opensource.org/licenses/mit-license.php  MIT License
 */

#include "qrcnv.h"

/* {{{ utility macro */

#define repeat(m, n) for ((m) = 0; (m) < (n); (m)++)

/* }}} */
/* {{{ symbol writing macro */

#define qrInitRow(filler) { \
	memset(rbuf, (filler), (size_t)rsize); \
	rptr = rbuf; \
}

#define qrWriteRow(m, n) { \
	wsize = (int)(rptr - rbuf); \
	for ((m) = 0; (m) < (n); (m)++) { \
		memcpy(sptr, rbuf, (size_t)wsize); \
		sptr += wsize; \
	} \
	if (wsize < rsize) { \
		*size -= (rsize - wsize) * (n); \
	} \
}

/*
 * シンボルを出力する際の定型作業をまとめたマクロ
 *
 * このマクロを呼び出す前に以下の4つのマクロをdefineし、
 * 呼び出した後はundefする
 *  qrWriteBOR()     行頭を書き込む
 *  qrWriteEOR()     行末を書き込む
 *  qrWriteBLM(m, n) 明モジュールを書き込む
 *  qrWriteDKM(m, n) 暗モジュールを書き込む
*/
#define qrWriteSymbol(qr, filler) { \
	/* 分離パターン (上) */ \
	if (sepdim > 0) { \
		qrInitRow(filler); \
		qrWriteBOR(); \
		qrWriteBLM(j, imgdim); \
		qrWriteEOR(); \
		qrWriteRow(i, sepdim); \
	} \
	for (i = 0; i < dim; i++) { \
		/* 行を初期化 */ \
		qrInitRow(filler); \
		/* 行頭 */ \
		qrWriteBOR(); \
		/* 分離パターン (左) */ \
		qrWriteBLM(j, sepdim); \
		/* シンボル本体 */ \
		for (j = 0; j < dim; j++) { \
			if (qrIsBlack((qr), i, j)) { \
				qrWriteDKM(jx, mag); \
			} else { \
				qrWriteBLM(jx, mag); \
			} \
		} \
		/* 分離パターン (右) */ \
		qrWriteBLM(j, sepdim); \
		/* 行末 */ \
		qrWriteEOR(); \
		/* 行をmag回繰り返し書き込む */ \
		qrWriteRow(ix, mag); \
	} \
	/* 分離パターン (下) */ \
	if (sepdim > 0) { \
		qrInitRow(filler); \
		qrWriteBOR(); \
		qrWriteBLM(j, imgdim); \
		qrWriteEOR(); \
		qrWriteRow(i, sepdim); \
	} \
}

/* }}} */
/* {{{ Structured append symbol writing macro */

#define qrsWriteSymbols(st, filler) { \
	for (k = 0; k < rows; k++) { \
		/* 分離パターン (上) */ \
		if (sepdim > 0) { \
			qrInitRow(filler); \
			qrWriteBOR(); \
			qrWriteBLM(j, xdim); \
			qrWriteEOR(); \
			qrWriteRow(i, sepdim); \
		} \
		for (i = 0; i < dim; i++) { \
			/* 行を初期化 */ \
			qrInitRow(filler); \
			/* 行頭 */ \
			qrWriteBOR(); \
			for (kx = 0; kx < cols; kx++) { \
				/* 分離パターン (左) */ \
				qrWriteBLM(j, sepdim); \
				/* シンボル本体 */ \
				if (order < 0) { \
					pos = k + rows * kx; \
				} else { \
					pos = cols * k + kx; \
				} \
				if (pos < (st)->num) { \
					for (j = 0; j < dim; j++) { \
						if (qrIsBlack((st)->qrs[pos], i, j)) { \
							qrWriteDKM(jx, mag); \
						} else { \
							qrWriteBLM(jx, mag); \
						} \
					} \
				} else { \
					qrWriteBLM(j, zdim); \
				} \
			} \
			/* 分離パターン (右) */ \
			qrWriteBLM(j, sepdim); \
			/* 行末 */ \
			qrWriteEOR(); \
			/* 行をmag回繰り返し書き込む */ \
			qrWriteRow(ix, mag); \
		} \
	} \
	/* 分離パターン (下) */ \
	if (sepdim > 0) { \
		qrInitRow(filler); \
		qrWriteBOR(); \
		qrWriteBLM(j, xdim); \
		qrWriteEOR(); \
		qrWriteRow(i, sepdim); \
	} \
}

