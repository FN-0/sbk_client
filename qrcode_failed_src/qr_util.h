/*
 * QR Code Generator Library: Header for Utility
 *
 * Core routines were originally written by Junn Ohta.
 * Based on qr.c Version 0.1: 2004/4/3 (Public Domain)
 *
 * @package     libqr
 * @author      Ryusuke SEKIYAMA <rsky0711@gmail.com>
 * @copyright   2006-2013 Ryusuke SEKIYAMA
 * @license     http://www.opensource.org/licenses/mit-license.php  MIT License
 */

#ifndef _QR_UTIL_H_
#define _QR_UTIL_H_

#include "qr.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

/*
 * Determine the module is a dark module or not.
 */
#define qrIsBlack(qr, i, j) (((qr)->symbol[(i)][(j)] & QR_MM_BLACK) != 0)

/*
 * Deallocate and set to NULL.
 */
#define qrFree(ptr) { if ((ptr) != NULL) { free(ptr); (ptr) = NULL; } }

/*
 * Current function name macro.
 */
extern const char *(*qrGetCurrentFunctionName)(void);
#if defined(__FUNCTION__)
#define _QR_FUNCTION ((qrGetCurrentFunctionName) ? qrGetCurrentFunctionName() : __FUNCTION__)
#elif defined(__func__)
#define _QR_FUNCTION ((qrGetCurrentFunctionName) ? qrGetCurrentFunctionName() : __func__)
#else
#define _QR_FUNCTION ((qrGetCurrentFunctionName) ? qrGetCurrentFunctionName() : "?")
#endif

/*
 * Maximum length of filename extensions.
 */
#define QR_EXT_MAX_LEN 4

/*
 * Constatns.
 */
extern const qr_vertable_t qr_vertable[];
extern const char *qr_eclname[];

/*
 * Functions for utility.
 */
const char *qrVersion(void);
const char *qrMimeType(int format);
const char *qrExtension(int format);
const char *qrStrError(int errcode);
void qrSetErrorInfo(QRCode *qr, int errnum, const char *param);
void qrSetErrorInfo2(QRCode *qr, int errnum, const char *param);
void qrSetErrorInfo3(QRCode *qr, int errnum, const char *fmt, ...);
int qrGetEncodedLength(QRCode *qr, int size);
int qrGetEncodedLength2(QRCode *qr, int size, int mode);
int qrGetEncodableLength(QRCode *qr, int size);
int qrGetEncodableLength2(QRCode *qr, int size, int mode);
int qrRemainedDataBits(QRCode *qr);

/*
 * Functions for checking datatype.
 */
int qrDetectDataType(const qr_byte_t *source, int size);
int qrStrPosNotNumeric(const qr_byte_t *source, int size);
int qrStrPosNotAlnum(const qr_byte_t *source, int size);
int qrStrPosNotKanji(const qr_byte_t *source, int size);
int qrStrPosNot8bit(const qr_byte_t *source, int size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _QR_UTIL_H_ */
