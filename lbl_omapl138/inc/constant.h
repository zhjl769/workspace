
//定义每次计算的块大小
#define IN_LENGTH			512
//定义FFT大小
#define NFFT               1024
#define N_LP                128

#define digital_delay		   0.0006335134                       
//0.000176522	


#ifndef PI
#define PI	3.14159265358979323846
#endif

#define MAXBUFFER 2338

//定义相关峰参数信息（形状，信噪比，噪声门限等）
struct  Para
{
	double snr_max;           //峰值门限
	int snr_abso;                //绝对信噪比
	int snr_rela;                 //向前虚警信噪比
	int snr_rela2;               //向后混响信噪比
	int Maikuan_l;             //峰宽下限
	int Maikuan_h;            //峰宽上限
	int w_all;                    //总峰值数量
	int w_before;              //前伪峰数量
	int w_after;                  //后伪峰数量
	int close_time;//单位ms      避混响时间
	int th_noise;//单位mV
}; 
//定义搜索结果参数信息（搜索标志，相关峰门限起点，时延等）
struct Sig
{
   double delay;
   double envelop_max;
   double noise_std;
   int corr_pos;
   int snr;
   int th_abso;
   int th_max[3]; 
   int th_num[3];
   int wid;
   char flag;  
};


