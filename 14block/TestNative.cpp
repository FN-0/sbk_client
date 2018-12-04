#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include "TestNative.h"

using namespace std;
using namespace cv;

#define COLORNUM 14 // 色块个数
#define PIXTHRESH 60
#define RADIR 10//取色块半径值

static const float detector[] = { 0.222699f,0.226992f,0.227205f,0.243592f,0.196159f,0.0501005f,0.013389f,0.0310156f,0.0898919f,0.291667f,0.181188f,0.0625607f,-0.0378959f,-0.137128f,-0.0424584f,0.00214961f,0.0822857f,0.342764f,-0.181057f,-0.00507539f,0.0601655f,0.232409f,0.000917722f,0.126489f,0.0190965f,-0.00507625f,-0.21978f,-0.0702351f,0.043702f,
0.0602118f,0.0854316f,0.246243f,0.0267782f,0.0304331f,0.00399833f,-0.13256f,0.238736f,0.0934262f,-0.0579984f,-0.182133f,-0.271536f,-0.122002f,-0.0137205f,0.172588f,0.261348f,0.258265f,0.201347f,-0.0236099f,-0.147143f,-0.287782f,-0.18956f,-0.0270704f,0.178298f,0.207874f,0.0885897f,0.0664065f,0.0194376f,-0.108791f,-0.189769f,-0.0726938f,0.00562732f,
0.0388089f,0.0572347f,0.131205f,0.0409106f,0.00747445f,-0.0550161f,-0.142841f,-0.0256071f,0.0375512f,0.042112f,0.131008f,0.293505f,0.0615724f,0.021947f,0.00525961f,0.0136445f,-0.0898181f,0.00964936f,0.0504358f,0.202038f,0.00537392f,-0.00237498f,0.022983f,0.0977779f,0.428684f,0.208827f,0.248071f,0.214778f,0.156208f,-0.228727f,-0.0373262f,-0.0030945f,
0.0677334f,0.342972f,-0.0452809f,-0.0691974f,-0.0939143f,-0.232961f,-0.217606f,-0.0917904f,-0.0329773f,0.263957f,0.127266f,0.0739863f,-0.069407f,-0.0854313f,-0.186542f,-0.198314f,-0.0870033f,-0.0361887f,0.104657f,0.0822325f,0.155436f,0.00318849f,-0.0155149f,-0.143868f,-0.19847f,-0.0573655f,0.0166142f,0.0397808f,0.360619f,0.0769465f,0.0306206f,-0.0216789f,
-0.193771f,-0.198594f,-0.0213518f,0.040863f,0.160328f,0.0815253f,0.179637f,-0.0725222f,-0.131798f,-0.254226f,-0.218312f,-0.0155254f,0.0459486f,0.061816f,0.426508f,0.0762828f,-0.00339404f,-0.0766806f,-0.235239f,-0.0527771f,0.086285f,0.111068f,0.0283586f,-0.0198798f,0.0570074f,0.107712f,0.107248f,-0.0401381f,-0.046635f,0.0958642f,0.148347f,0.136157f,
-0.0653493f,0.101932f,0.114708f,0.0835736f,-0.0749176f,-0.0783006f,0.139497f,0.162691f,0.0620963f,0.00131469f,0.0483982f,0.113186f,0.102846f,-0.0828798f,-0.0580013f,0.0822995f,0.136895f,0.084943f,-0.0718655f,0.126547f,0.115103f,0.11168f,-0.0564525f,-0.0911939f,-0.0334194f,-0.025363f,2.23069e-06f,0.295137f,-0.0530096f,-0.0514571f,-0.0543266f,-0.110615f,
-0.214574f,-0.110744f,-0.100429f,0.145731f,0.146857f,-0.00297331f,-0.154585f,-0.130552f,-0.212129f,-0.129728f,-0.0560613f,-0.0576687f,-0.0543402f,0.349848f,-0.0157419f,-0.0455389f,-0.049957f,-0.126658f,-0.236783f,-0.141648f,-0.159338f,0.184135f,0.128368f,0.0105157f,-0.131077f,-0.135727f,-0.235088f,-0.188605f,0.00113284f,0.0373701f,0.144165f,0.0225716f,
0.281671f,0.0784448f,-0.00612959f,-0.109261f,-0.0628522f,0.029944f,0.040697f,0.00294681f,0.132787f,0.0487328f,0.0299169f,0.017734f,0.0327345f,0.107693f,0.0252658f,0.00613806f,-0.0150089f,0.173883f,0.146336f,0.296951f,0.304182f,0.339736f,0.328552f,0.0995287f,0.000849605f,-0.0755888f,-0.170443f,-0.0507785f,0.0730488f,0.258325f,0.307305f,0.0223139f,
0.0353826f,0.0108433f,-0.0756242f,-0.215846f,-0.0810222f,-0.0199466f,0.00985586f,0.0484515f,0.0433528f,0.00647657f,0.0255011f,-0.0118075f,-0.141251f,-0.0513324f,-0.00882516f,0.023491f,0.0518935f,0.244651f,0.142252f,-0.0284173f,-0.131427f,-0.283876f,-0.164176f,-0.0605627f,0.151945f,0.243249f,0.231098f,0.130739f,-0.0326017f,-0.156713f,-0.238616f,
-0.128343f,0.0285466f,0.26141f,0.291879f,-0.201578f,-0.0890812f,-0.0547789f,0.049702f,0.244931f,0.031582f,0.000369234f,-0.0114043f,-0.186871f,-0.144506f,-0.0630728f,0.00273548f,0.234391f,-0.00661459f,0.12163f,-0.0251262f,-0.0678464f,-0.181942f,0.28898f,0.150206f,0.0576561f,-0.0046641f,0.0770411f,-0.0186276f,0.0490225f,0.129126f,0.334683f,0.162901f,
0.346359f,0.352919f,0.335414f,0.305493f,0.0334156f,0.0332071f,0.0135419f,-0.0141329f,-3.6181f };

enum DetectType { URO = 0, BLO, BIL, KET, CA, LEU, GLU, PRO, PH, CRE, NIT, SG, VC, MCA };
string BlockPath[] = { "URO","BLO", "BIL", "KET", "CA", "LEU", "GLU", "PRO", "PH", "CRE", "NIT", "SG", "VC", "MCA" };

enum ColorType { H = 0, S, I, Y, CB, CR };
string ColorName[] = { "H","S","I","Y","CB","CR" };

typedef struct _SigVal
{
	string sig;
	double val;
}SigVal;

void Sort_Point_Y(vector<Point> &data)
{
	Point tmp;
	for (int i = 0; i < data.size() - 1; i++) {
		for (int j = i + 1; j < data.size(); j++) {
			if (data[i].y > data[j].y) {
				tmp = data[i];
				data[i] = data[j];
				data[j] = tmp;
			}
		}
	}
}

int Get_Roi(const Mat &im, Mat &roi)
{
	int w = im.cols;
	int h = im.rows;
	int *bk_stat = new int[w];
	memset(bk_stat, 0, sizeof(int)*w);
	
	int all_num = 0;
	for (int n = 0; n < w; n++) {
		for (int m = 0; m < h; m++) {
			// 获取像素三通道中的最大值
			int pmax = im.data[(m*w + n) * 3];
			for (int c = 1; c < 3; c++) {
				if (pmax < im.data[(m*w + n) * 3 + c])
					pmax = im.data[(m*w + n) * 3 + c];
			}

			if (pmax < PIXTHRESH) {
				bk_stat[n]++;
				all_num++;
			}
		}
	}

	int sidx = 0;
	float percent = 0;
	while (sidx < w - 1) {
		percent += bk_stat[sidx];
		if (percent / all_num >= 0.05) break;
		sidx++;
	}

	int eidx = w - 1;
	percent = 0;
	while (eidx > sidx) {
		percent += bk_stat[eidx];
		if (percent / all_num >= 0.05) break;
		eidx--;

	}

	if (sidx >= w || eidx <= 0 || sidx >= eidx) return -1;
	Rect roi_rt = Rect(sidx, 0, eidx - sidx + 1, h);
	roi = im(roi_rt).clone();

	delete bk_stat;
	return sidx;
}

void Color_Cluster(vector<Point> &cnt, vector<Point> &clu)
{
	int k = 0;
	while (k < cnt.size() - 1) {
		Rect r1 = Rect(cnt[k].x - 16, cnt[k].y - 16, 32, 32);
		int ave_x = cnt[k].x;
		int ave_y = cnt[k].y;
		int num = 1;

		int j;
		for (j = k + 1; j < cnt.size(); j++) {
			Rect r2 = Rect(cnt[j].x - 16, cnt[j].y - 16, 32, 32);
			float per = 1.0f*(r1&r2).area() / r1.area();
			if (per > 0.1) {
				ave_x += cnt[j].x;
				ave_y += cnt[j].y;
				num++;
			}
			else
				break;
		}

		clu.push_back(Point(ave_x / num, ave_y / num));
		k = j;
	}
}

void Calc_New_Cluster(vector<Point> &clu, vector<Point> &nclu)
{
	vector<int>vlen;
	for (int k = 0; k < clu.size(); k++) {
		Point cnt = Point(clu[k].x, clu[k].y);
		if (k >= 1) {
			vlen.push_back(clu[k].y - clu[k - 1].y);
		}
	}
	sort(vlen.begin(), vlen.end());

	// 取中间相对正确的部分
	int mov_y = 0;
	for (int k = 3; k < vlen.size() - 3; k++) {
		mov_y += vlen[k];
	}
	mov_y /= (vlen.size() - 6);

	//vector<Point> ncolor;
	nclu.push_back(clu[0]);
	for (int k = 1; k < clu.size(); k++) {
		int m = clu[k].y - clu[k - 1].y;

		if (1.0f * m / mov_y >= 1.5f) {
			nclu.push_back(0.5f*(clu[k] + clu[k - 1]));
		}
		nclu.push_back(clu[k]);
		if (nclu.size() >= COLORNUM) break;
	}
	while (nclu.size() < COLORNUM) {
		Point pt;
		int nc_size = nclu.size();
		pt.x = 2 * nclu[nc_size - 1].x - nclu[nc_size - 2].x;
		pt.y = nclu[nc_size - 1].y + mov_y;
		nclu.push_back(pt);
	}
}


void Get_Color_Space(Mat &im, vector<Point> &ncolor)
{
	//namedWindow("src", 0);
	Mat roi;
	int sidx = Get_Roi(im, roi);
	if(sidx <= 0) return;

	vector<Rect> found;//矩形框数组
	vector<Point>found_cnt;
	HOGDescriptor myHOG(Size(32, 32), Size(16, 16), Size(8, 8), Size(8, 8), 9);
	vector<float> myDetector(detector, detector + sizeof(detector) / sizeof(detector[0]));
	myHOG.setSVMDetector(myDetector);
	myHOG.detectMultiScale(roi, found, 0, Size(4, 4), Size(0, 0), 1.01, 0.8);
	for (int k = 0; k < found.size(); k++) {
		Rect r = found[k];
		int j;
		for (j = 0; j < found.size(); j++)
			if (j != k && (r & found[j]) == r) break;
		if (j == found.size())
			found_cnt.push_back(Point(r.x + r.width / 2, r.y + r.height / 2));
	}
	//cout << "target num: " << found_cnt.size() << endl;
	//cout << "use time: " << (double)(clock() - start) / CLOCKS_PER_SEC << " s" << endl;

	vector<Point> color;// 对同类的色块聚类处理，理想情况大小为14
	//vector<Point> ncolor;// 若color不等于14，深度处理后存入ncolor中
	Sort_Point_Y(found_cnt);
	Color_Cluster(found_cnt, color);
	Calc_New_Cluster(color, ncolor);

	Mat frame = im.clone();
	for (int k = 0; k < ncolor.size(); k++) {
		ncolor[k].x += sidx;
		circle(frame, ncolor[k], 7, Scalar(255, 255, 255), -1);
	}
	imwrite("/usr/14block/rst/blk_img.jpg",frame);
}

int AveBGR(Mat &im, int &aveB, int &aveG, int &aveR)
{
	aveB = 0;
	aveG = 0;
	aveR = 0;
	unsigned char *pdr = im.data;
	int szIm = im.rows*im.cols;
	for (int n = 0; n < im.rows; n++)
	{
		for (int m = 0; m < im.cols; m++)
		{
			aveB += *pdr++;
			aveG += *pdr++;
			aveR += *pdr++;
		}
	}
	aveB = (int)(0.5 + 1.0*aveB / szIm);
	aveG = (int)(0.5 + 1.0*aveG / szIm);
	aveR = (int)(0.5 + 1.0*aveR / szIm);

	return 1;
}

// http://blog.csdn.net/andrewseu/article/details/49534575
int BGR2HSI(int B, int G, int R, double &h, double &s, double &fi)
{
	int iMax, iMin;
	iMax = max(max(B, G), R);
	iMin = min(min(B, G), R);

	fi = iMax;
	s = 0;
	if (iMax != 0)
	{
		s = 1.0 - 1.0*iMin / fi;
	}

	if (iMax == R)
	{
		if (G >= B)
		{
			h = 60.0 * (G - B) / (fi - iMin);
		}
		else
		{
			h = 360 + 60.0 * (G - B) / (fi - iMin);
		}
	}
	else if (iMax == G)
	{
		h = 120 + 60.0 * (B - R) / (fi - iMin);
	}
	else
	{
		h = 240 + 60.0 * (R - G) / (fi - iMin);
	}
	return 1;
}


// http://blog.sina.com.cn/s/blog_5713096b0100059i.html
int BGR2YCbCr(int B, int G, int R, double &y, double &cb, double &cr)
{
	y = 0.257 * R + 0.504 * G + 0.098 * B + +16;
	cb = -0.148 * R - 0.291 * G + 0.439 * B + 128;
	cr = 0.439 * R - 0.368 * G - 0.071 * B + 128;

	return 1;
}

/* JNI reference here: https://www.cnblogs.com/Seiyagoo/p/3496834.html */
/* You should use `javac -h` instead of `javah` when `javah` could not be found */
/* 'javac -h' usage here: https://stackoverflow.com/questions/46577196/javac-no-source-files-when-using-h-option */
/* shared library reference here: https://www.cnblogs.com/ifantastic/p/3526237.html */
JNIEXPORT jstring JNICALL Java_com_fengneng_controller_TestNative_testNative
	(JNIEnv *env, jclass cls, jstring jstr) 
//int main(int argc, char ** argv)
{
	//Mat& src = *(Mat *)source;	
	const char* path;
    path = env->GetStringUTFChars(jstr, JNI_FALSE);
    if(path == NULL) {
        return NULL;
    }	
	string blkRst = "";
	Mat src = imread(path);
	ifstream ifs("/usr/14block/thr.txt");
	vector<vector<SigVal>> vsv;
	vector<int> vdx;
	for (int i = 0; i < COLORNUM; i++){
		int id, num;
		ifs >> id >> num;
		vector<SigVal> vt(num + 1);
		for (int k = 0; k < num + 1; k++){
			ifs >> vt[k].sig;
			//cout << vt[k].sig << endl;
		}
		for (int k = 0; k < num; k++){
			ifs >> vt[k].val;
			//cout << vt[k].val << endl;
		}

		vsv.push_back(vt);
		vdx.push_back(id);
	}
	ifs.close();


	vector<Point> ct;
	ct.clear();
	Get_Color_Space(src, ct);
	if (ct.size() != COLORNUM) {
		//cout << "Can't Detect the QRCode!" << endl;
		//continue;
		imshow("Image",src);
	}
	else{
		vector<vector<double>> blk;
		for (int idx = 0; idx < ct.size(); idx++) {
			vector<double> vCol(6);
			Rect rt = Rect(ct[idx].x - RADIR, ct[idx].y - RADIR, 2*RADIR, 2*RADIR);
			Mat roi = src(rt).clone();
			// B,G,R color space
			int aveB = 0, aveG = 0, aveR = 0;
			AveBGR(roi, aveB, aveG, aveR);
			// H,S,I color space
			double h, s, fi;
			BGR2HSI(aveB, aveG, aveR, h, s, fi);
			vCol[0] += h;
			vCol[1] += s;
			vCol[2] += fi;
			// Y, Cb, Cr color space
			double y, cb, cr;
			BGR2YCbCr(aveB, aveG, aveR, y, cb, cr);
			vCol[3] += y;
			vCol[4] += cb;
			vCol[5] += cr;
			blk.push_back(vCol);
		}
		//string blkRst = ;
		for (int idx = 0; idx < ct.size(); idx++) {
			int i = vdx[idx];
			double curVal = blk[idx][i];
			int up;
			if (vsv[idx][0].val < vsv[idx][1].val)
				up = 1;
			else
				up = -1;
			int j;
			for (j = 0; j < vsv[idx].size(); j++) {
				if (up*curVal < up*vsv[idx][j].val) break;
			}
			if (j == vsv[idx].size())
				j = vsv[idx].size() - 1;
			// return jstring value
			string ostr = BlockPath[idx] + ": " + vsv[idx][j].sig + "\n";
			blkRst += ostr;
		}
		
	}

	const char* cstr = blkRst.c_str();
	jstring rtstr = env->NewStringUTF(cstr);

	return rtstr;
	//cout << blkRst << endl;
	//return 0;
}

