
//����ÿ�μ���Ŀ��С
#define IN_LENGTH			512
//����FFT��С
#define NFFT               1024
#define N_LP                128

#define digital_delay		  0	


#ifndef PI
#define PI	3.14159265358979323846
#endif

#define MAXBUFFER 2338

//������ط������Ϣ����״������ȣ��������޵ȣ�
struct  Para
{
	double snr_max;
	int snr_abso;
	int snr_rela;
	int snr_rela2;
	int Maikuan_l;
	int Maikuan_h;
	int w_all;
	int w_before;
	int w_after;
	int close_time;//��λms
	int th_noise;//��λmV
}; 
//�����������������Ϣ��������־����ط�������㣬ʱ�ӵȣ�
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


