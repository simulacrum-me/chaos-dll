#pragma once

#ifdef CHAOS_EXPORTS
#define CHAOS_API __declspec(dllexport)
#else
#define CHAOS_API __declspec(dllimport)
#endif

extern "C" CHAOS_API void render_map_tile(int zoom, int y, int x, int max_iter, double epsilon, int averaging, long* p_array);
extern "C" CHAOS_API void calculate_bifurcation_column(int column,int column_resolution, double omega, int num_events, int num_conv_it, long* p_array);