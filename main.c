#include <stdio.h>
#include <stdlib.h> 
#include "stdint.h"
 
#include "v4l2.c"

#if defined ( __cplusplus)
extern "C"
{
#include "x264.h"
};
#else
#include "x264.h"
#endif


 
int main(int argc, char** argv)
{
 
	int ret;
	int y_size;
	int i,j;
	char *yuvbuf = NULL;
	unsigned int bufLen = 0;
	unsigned int frame_num = 1000;
	char *dev_name = "/dev/video1";
	
	if(argc > 1)
	{
		dev_name = argv[1];
	}
	
	FILE* fp_dst = fopen("xianlee.h264", "wb");
	
	int csp=X264_CSP_I420;
	int width=640,height=480;

	int iNal   = 0;
	x264_nal_t* pNals = NULL;
	x264_t* pHandle   = NULL;
	x264_picture_t* pPic_in = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	x264_picture_t* pPic_out = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	x264_param_t* pParam = (x264_param_t*)malloc(sizeof(x264_param_t));

	cameraInit(dev_name);
	        
	 //Check
	 if(fp_dst == NULL){
			   printf("Error open files.\n");
			   return -1;
	 }

	 x264_param_default(pParam);
	 pParam->i_width   = width;
	 pParam->i_height  = height;
	 
	 /*
	 //Param
	 pParam->i_log_level  = X264_LOG_DEBUG;
	 pParam->i_threads  = X264_SYNC_LOOKAHEAD_AUTO;
	 pParam->i_frame_total = 0;
	 pParam->i_keyint_max = 10;
	 pParam->i_bframe  = 5;
	 pParam->b_open_gop  = 0;
	 pParam->i_bframe_pyramid = 0;
	 pParam->rc.i_qp_constant=0;
	 pParam->rc.i_qp_max=0;
	 pParam->rc.i_qp_min=0;
	 pParam->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
	 pParam->i_fps_den  = 1;
	 * */
	 pParam->i_fps_num  = 25;
	 //pParam->i_timebase_den = pParam->i_fps_num;
	 //pParam->i_timebase_num = pParam->i_fps_den;
	 
	 pParam->i_csp=csp;
	 x264_param_apply_profile(pParam, x264_profile_names[5]);
	
	 pHandle = x264_encoder_open(pParam);

	 x264_picture_init(pPic_out);
	 x264_picture_alloc(pPic_in, csp, pParam->i_width, pParam->i_height);

	 //ret = x264_encoder_headers(pHandle, &pNals, &iNal);

	 y_size = pParam->i_width * pParam->i_height;
	
	 //Loop to Encode
	 for( i=0;i<frame_num;i++){
		getFrame(&yuvbuf, &bufLen);
		
		if( bufLen <= 0 || yuvbuf == NULL)
		{
			printf("Error ! \n");
			break;
		}
		
		printf("Buf size: %d  y_size : %d! \n",bufLen,y_size);
		
		memcpy(pPic_in->img.plane[0], yuvbuf, y_size);         //Y
		memcpy(pPic_in->img.plane[1], (yuvbuf+y_size), y_size/4);     //U
		memcpy(pPic_in->img.plane[2], (yuvbuf+y_size+y_size/4), y_size/4);     //V
		
		printf("copy done ! \n");
		
		pPic_in->i_pts = i;

		ret = x264_encoder_encode(pHandle, &pNals, &iNal, pPic_in, pPic_out);
		if (ret< 0){
			printf("Error.\n");
			return -1;
		}


		for ( j = 0; j < iNal; ++j){
			fwrite(pNals[j].p_payload, 1, pNals[j].i_payload, fp_dst);
		}
	 }
	 i=0;
	 //flush encoder
	while(1){
		ret = x264_encoder_encode(pHandle, &pNals, &iNal, NULL, pPic_out);
		if(ret==0){
			break;
		}
		printf("Flush 1 frame.\n");
		for (j = 0; j < iNal; ++j){
			fwrite(pNals[j].p_payload, 1, pNals[j].i_payload, fp_dst);
		}
		i++;
	}
	
	cameraClose();
	 x264_picture_clean(pPic_in);
	 x264_encoder_close(pHandle);
	 pHandle = NULL;

	 free(pPic_in);
	 free(pPic_out);
	 free(pParam);

	 fclose(fp_dst);

	 return 0;
}
