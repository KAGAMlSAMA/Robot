#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "amazon2_sdk.h"
#include "graphic_api.h"

#define AMAZON2_GRAPHIC_VERSION		"v0.6"

void show_help(void)
{
	printf("================================================================\n");
	printf("Graphic API Example (Ver : %s)\n", AMAZON2_GRAPHIC_VERSION);
	printf("================================================================\n");
	printf("h : show this message\n");
	printf("a : direct camera display on\n");
	printf("d : direct camera display off\n");
	printf("0 : flip the front buffer and the back buffer\n");
	printf("1 : clear the back buffer \n");
	printf("2 : draw_rectfill(10, 200, 100, 100, MAKE_COLORREF(255, 0, 0)); \n");
	printf("3 : draw_rectfill(110, 200, 100, 100, MAKE_COLORREF(0, 255, 0)); \n");
	printf("4 : draw_rectfill(210, 200, 100, 100, MAKE_COLORREF(0, 0, 255)); \n");
	printf("5 : bmp(/root/img/AMAZON2.bmp) load \n");
	printf("6 : bmp(/root/img/AMAZON2.bmp) draw \n");
	printf("m : Mean filtering \n");
	printf("c : Color detection \n");
	printf("i : Color inversion \n");
	printf("q : exit \n");
	printf("================================================================\n");
}
static void color_detection(void)
{
	// write your own code
	U16 rgb565;
	int i = 15 , x, y;
	int blue = 0, green = 0, red = 0;
	int sblue = 0, sgreen = 0, sred = 0 , syellow = 0, sblack = 0, swhite=0;
	int pidx;
	int pixn;
	int m;
    pixn = 3000;
	U16* fpga_videodata = (U16*)malloc(180 * 120 * 2);
	printf("Color detection start\n");
	while (i--)
	{
		sblue = 0; sgreen = 0; sred = 0; syellow = 0; sblack = 0;swhite=0;
		clear_screen();
		read_fpga_video_data(fpga_videodata);

		for (pidx = 0; pidx < 180 * 120; pidx++) {
			red = (fpga_videodata[pidx] & 0xF800) >> 10; //(6bits)
			green = (fpga_videodata[pidx] & 0x7E0) >> 5; //(6bits)
			blue = (fpga_videodata[pidx] & 0x1F) << 1; //(6bits)
			if(red >=28 && green >=28 && blue <20) syellow++;
			else if(red >=28 && blue<20 && green <20) sred++;
			else if(blue >= 28 && red < 20 && green<20) sblue++;
			else if(green >=28 && blue <20 && red <20) sgreen++;
			else if(green >=28 && blue >=28 && red>=28) swhite++;
			
			
			
			
			//rgb565 = ((red >> 1) << 11) | (green << 5) | (blue >> 1);
			//fpga_videodata[pidx] = rgb565;
		}
			
			
			if (syellow >= pixn/2) printf("Yellow Object!");
			else if (sred >= pixn/2 && sgreen<=pixn/3 && sblue<=pixn/3) printf("Red Object!/n");
			else if (sgreen >= pixn/2 && sred <= pixn/3 && sblue<=pixn/3) printf("Green Object!/n");
			else if (sblue >= pixn/2 && sred <= pixn/3 && sgreen <=pixn/3 ) printf("Blue Object!/n");
		
		draw_fpga_video_data_full(fpga_videodata);
		flip();
	}
	printf("Color detection end  \n");
	free(fpga_videodata);
}

static void color_inversion(void)
{
	U16 rgb565;
	int i = 300, x, y;
	int blue = 0, green = 0, red = 0;
	int pidx;
	U16* fpga_videodata = (U16*)malloc(180 * 120 * 2);
	printf("Color inversion start\n");
	while (i--)
	{
		clear_screen();

		x = rand() % 300;
		y = rand() % 460;
		draw_rectfill(x, y, 20, 20, MAKE_COLORREF(255, 0, 0));
		x = rand() % 300;
		y = rand() % 460;
		draw_rectfill(x, y, 20, 20, MAKE_COLORREF(0, 255, 0));
		read_fpga_video_data(fpga_videodata);

		for (pidx = 0; pidx < 180 * 120; pidx++) {
			red = (fpga_videodata[pidx] & 0xF800) >> 10; //(6bits)
			green = (fpga_videodata[pidx] & 0x7E0) >> 5; //(6bits)
			blue = (fpga_videodata[pidx] & 0x1F) << 1; //(6bits)
			red = 63 - red; green = 63 - green; blue = 63 - blue;
			rgb565 = ((red >> 1) << 11) | (green << 5) | (blue >> 1);
			fpga_videodata[pidx] = rgb565;
		}
		draw_fpga_video_data_full(fpga_videodata);
		flip();
	}
	printf("Color inversion end\n");
	free(fpga_videodata);
}

static void mean_filtering(void)
{
	U16 rgb565;
	int i = 50, x = 0, y = 0, w = 180;
	int blue = 0, green = 0, red = 0;
	int pidx, h_idx, w_idx, col_idx, row_idx;
	int red_col_sum[11], green_col_sum[11], blue_col_sum[11];
	U16* fpga_videodata = (U16*)malloc(180 * 120 * 2);
	U16* temp_buffer = (U16*)malloc(180 * 120 * 2);
	printf("Mean filtering start\n");
	while (i--)
	{
		clear_screen();
		
		x = rand() % 300;
		y = rand() % 460;
		draw_rectfill(x, y, 20, 20, MAKE_COLORREF(255, 0, 0));
		x = rand() % 300;
		y = rand() % 460;
		draw_rectfill(x, y, 20, 20, MAKE_COLORREF(0, 255, 0));
		read_fpga_video_data(fpga_videodata);
		
		//mean filtering
		
		for (h_idx = 5; h_idx < 115; h_idx++) {
			w_idx = 5;
			for (col_idx = 0; col_idx < 11; col_idx++) {
				red_col_sum[col_idx] = 0; blue_col_sum[col_idx] = 0; green_col_sum[col_idx] = 0;
				for (row_idx = 0; row_idx < 11; row_idx++) {
					pidx = col_idx + w * (row_idx + h_idx - 5);
					rgb565 = fpga_videodata[pidx];
					red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
					green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
					blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				}
			}
			red = 0;	green = 0;	blue = 0;
			for (col_idx = 0; col_idx < 11; col_idx++) {
				red += red_col_sum[col_idx];
				green += green_col_sum[col_idx];
				blue += blue_col_sum[col_idx];
			}
			red = red / 121;
			green = green / 121;
			blue = blue / 121;
			rgb565 = ((red >> 1) << 11) | (green << 5) | (blue >> 1);
			pidx = h_idx * w + w_idx;
			temp_buffer[pidx] = rgb565;
			for (w_idx = 6; w_idx < 175; w_idx++) {
				col_idx = (w_idx - 6) % 11;
				red_col_sum[col_idx] = 0; blue_col_sum[col_idx] = 0; green_col_sum[col_idx] = 0;
				//=====================
				row_idx = 0;
				pidx = w_idx + w * (row_idx + h_idx - 5);
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				pidx += w;
				rgb565 = fpga_videodata[pidx];
				red_col_sum[col_idx] += (rgb565 & 0xF800) >> 10;
				green_col_sum[col_idx] += (rgb565 & 0x7E0) >> 5;
				blue_col_sum[col_idx] += (rgb565 & 0x1F) << 1;
				row_idx++;
				//====================

				red = 0;	green = 0;	blue = 0;
				//===========================
				red += red_col_sum[0];
				green += green_col_sum[0];
				blue += blue_col_sum[0];
				red += red_col_sum[1];
				green += green_col_sum[1];
				blue += blue_col_sum[1];
				red += red_col_sum[2];
				green += green_col_sum[2];
				blue += blue_col_sum[2];
				red += red_col_sum[3];
				green += green_col_sum[3];
				blue += blue_col_sum[3];
				red += red_col_sum[4];
				green += green_col_sum[4];
				blue += blue_col_sum[4];
				red += red_col_sum[5];
				green += green_col_sum[5];
				blue += blue_col_sum[5];
				red += red_col_sum[6];
				green += green_col_sum[6];
				blue += blue_col_sum[6];
				red += red_col_sum[7];
				green += green_col_sum[7];
				blue += blue_col_sum[7];
				red += red_col_sum[8];
				green += green_col_sum[8];
				blue += blue_col_sum[8];
				red += red_col_sum[9];
				green += green_col_sum[9];
				blue += blue_col_sum[9];
				red += red_col_sum[10];
				green += green_col_sum[10];
				blue += blue_col_sum[10];
				//==================================
				red = red / 121; green = green / 121; blue = blue / 121;
				rgb565 = ((red >> 1) << 11) | (green << 5) | (blue >> 1);
				pidx = h_idx * w + w_idx;
				temp_buffer[pidx] = rgb565;
			}
		}
		draw_fpga_video_data_full(temp_buffer);
		flip();
	}
	free(fpga_videodata);
	free(temp_buffer);
	printf("Mean filtering end\n");
}

void init(void)
{
	show_help();
	if (open_graphic() < 0) {
		return -1;
	}
}

int main(int argc, char **argv)
{
	BOOL b_loop = TRUE;
	SURFACE* bmpsurf = 0;
	U16* fpga_videodata = (U16*)malloc(180 * 120 * 2);
	init();


	while (b_loop)
	{
		int ch = getchar();
		switch (ch)
		{
		case 'q':
		case 'Q':
			b_loop = FALSE;
			break;
		case 'a':
		case 'A':
			printf("direct camera display on\n");
			direct_camera_display_on();
			break;
		case 'd':
		case 'D':
			printf("direct camera display off\n");
			direct_camera_display_off();
			break;
		case '0':
			if(direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			printf("flip\n");
			flip();
			break;
		case '1':
			if(direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			printf("clear_screen\n");
			clear_screen();
			break;
		case '2':
			if(direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			printf("draw_rectfill(10, 200, 100, 100, MAKE_COLORREF(255, 0, 0));\n");
			draw_rectfill(10, 200, 100, 100, MAKE_COLORREF(255, 0, 0));
			break;
		case '3':
			if(direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			printf("draw_rectfill(110, 200, 100, 100, MAKE_COLORREF(0, 255, 0));\n");
			draw_rectfill(110, 200, 100, 100, MAKE_COLORREF(0, 255, 0));
			break;
		case '4':
			if(direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			printf("draw_rectfill(210, 200, 100, 100, MAKE_COLORREF(0, 0, 255));\n");
			draw_rectfill(210, 200, 100, 100, MAKE_COLORREF(0, 0, 255));
			break;
		case '5':
			if (bmpsurf != 0) 
			{
				printf("image wad already loaded\n ");

			}
			else
			{
				if(direct_camera_display_stat() > 0) {
					printf("direct camera display on\n");
					printf("please direct camera diplay off\n");
					break;
				}
				printf("bmp(/root/img/AMAZON2.bmp) load\n");
				bmpsurf = loadbmp("/root/img/AMAZON2.bmp");
			}
			break;
		case '6':
			if (bmpsurf == 0)
			{
				printf("bmp is not loaded yet\n");
			}
			else
			{
				if(direct_camera_display_stat() > 0) {
					printf("direct camera display on\n");
					printf("please direct camera diplay off\n");
					break;
				}
				printf("bmp(/root/img/AMAZON2.bmp) draw\n");
				draw_surface(bmpsurf, 10, 300);
			}
			break;
		case 'm':
		case 'M':
			if(direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			mean_filtering();
			break;
		case 'i':
		case 'I':
			if (direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			color_inversion();
			break;
		case 'c':
		case 'C':
			if (direct_camera_display_stat() > 0) {
				printf("direct camera display on\n");
				printf("please direct camera diplay off\n");
				break;
			}
			color_detection();
			break;
		case 'h':
		case 'H':
			show_help();
		default:
			break;
		}
	}
	free(fpga_videodata);
	if (bmpsurf != 0)
		release_surface(bmpsurf);
	close_graphic();
	return 0;
}

