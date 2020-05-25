// chaos.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "chaos.h"
#define _USE_MATH_DEFINES 
#define end_tile_size 256
#include <math.h>
#include <random>

struct array2d{
	long * data;
	int w, h;
	array2d(long* p_array,int width,int height){
		data = p_array;
		w = width;
		h = height;
	}

	long& operator()(int x, int y){
		return data[y*w + x];
	}};

void render_map_tile(int zoom, int y, int x, int max_iter, double epsilon, int averaging, long* p_array) {
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	double max_x = 1;
	double max_y = 4 * M_PI;
	int tile_size = end_tile_size * 2;
	double dimension = pow(2.0, zoom);
	double dx = max_x / (tile_size*dimension);
	double dy = max_y / (tile_size*dimension);
	double tile_x_dim = max_x / dimension;
	double tile_y_dim = max_y / dimension;
	array2d z_2d = array2d(p_array,tile_size,tile_size);
	for (int x_coord = 0; x_coord < tile_size; x_coord++) {
		double omega = x*tile_x_dim + dx*x_coord;
		for (int y_coord = 0; y_coord < tile_size; y_coord++) {
			double K = y*tile_y_dim + dy*y_coord;
			int* it_arr = new int[averaging];
			for (int av_i = 0; av_i < averaging; av_i++){
				int curr_it = max_iter;
				double z0 = distribution(generator);
				double z = z0;
				for (int it_index = 0; it_index < max_iter; it_index++) {
					z = z + omega - K * sin(M_PI * 2 * z) / (2 * M_PI);
					if (fmod(fabs(z - z0) , 1) < epsilon) {
						curr_it = it_index;
						break;
						}
				}
					it_arr[av_i] = curr_it;
							
				}
			int sum = 0;
			for (int i = 0; i < averaging; i++){
				sum += it_arr[i];
			}
			delete[] it_arr;
			z_2d(x_coord, y_coord) = sum/averaging;
			}
		}
	}

void calculate_bifurcation_column(int column, int column_resolution,double omega, int num_events, int num_conv_it, long* p_array) {
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	double K_max = 2 * M_PI;
	int bins = column_resolution;
	double dK = K_max / bins;
	double dx = 1.0 / bins;
	for (int event_index = 0; event_index < num_events; event_index++) {
		double z = distribution(generator);
		for (int conv_index = 0; conv_index < num_conv_it; conv_index++) {
			z = z + omega - column*dK * sin(M_PI * 2 * z) / (2 * M_PI);
			z = fmod(z, 1);
		}
		int z_index = z / dx;
		if (z_index>=0 && z_index < bins){
			p_array[z_index]++;
		}
	}	
}

