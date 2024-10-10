/********************************************************
 * Kernels to be optimized for the OS&C prflab.
 * Acknowledgment: This lab is an extended version of the
 * CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "smooth.h" // helper functions for naive_smooth
#include "blend.h"  // helper functions for naive_blend
#include <pthread.h>
#include <limits.h>
#include <immintrin.h>

/* 
 * Please fill in the following struct
 */
student_t student = {
    "macj",             /* ITU alias */
    "Mads Christian Nørklit Jensen",    /* Full name */
    "macj@itu.dk", /* Email address */
};

/******************************************************************************
 * ROTATE KERNEL
 *****************************************************************************/

// Your different versions of the rotate kernel go here
char my_rotate_descr[] = "my_rotate: Fixing writes to dst to improve memory locality, done the loop interchange";
void my_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
        for (j = 0; j < dim; j++) // Interchanging the loops
            for (i = 0; i < dim; i++) 
                dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

char my_rotate2_descr[] = "my_rotate2: Transpose and exchange rows, with loop interchange + move calculation out of the loop";
void my_rotate2(int dim, pixel *src, pixel *dst) 
{
    int i, j, x;
        for (j = 0; j < dim; j++)  // Outer loop over columns
        {
            x = dim-1-j; // Calcuating row index for the rotated position (flipping rows)
            for (i = 0; i < dim; i++) // Inner loop over rows
            {
                dst[RIDX(x, i, dim)] = src[RIDX(i, j, dim)]; // Transpose and rotate
            }
        }
}

char my_rotate3_descr[] = "my_rotate3: With loop unrolling";
void my_rotate3(int dim, pixel *src, pixel *dst) 
{
    int i, j, x;
        for (j = 0; j < dim; j++) 
        {
            x = dim-1-j; 
            for (i = 0; i < dim; i+= 8) 
            {
                dst[RIDX(x, i, dim)] = src[RIDX(i, j, dim)]; 
                if (i + 1 < dim) dst[RIDX(x, i+1, dim)] = src[RIDX(i+1, j, dim)];
                if (i + 2 < dim) dst[RIDX(x, i+2, dim)] = src[RIDX(i+2, j, dim)];
                if (i + 3 < dim) dst[RIDX(x, i+3, dim)] = src[RIDX(i+3, j, dim)];
                if (i + 4 < dim) dst[RIDX(x, i+4, dim)] = src[RIDX(i+4, j, dim)];
                if (i + 5 < dim) dst[RIDX(x, i+5, dim)] = src[RIDX(i+5, j, dim)];
                if (i + 6 < dim) dst[RIDX(x, i+6, dim)] = src[RIDX(i+6, j, dim)];
                if (i + 7 < dim) dst[RIDX(x, i+7, dim)] = src[RIDX(i+7, j, dim)];
            }
        }
}

char my_rotate4_descr[] = "my_rotate4: Moving another calculation out of the loop";
void my_rotate4(int dim, pixel *src, pixel *dst) 
{
    int i, j, x, y;
        y = dim-1;
        for (j = 0; j < dim; j++) 
        {
            x = y-j; 
            for (i = 0; i < dim; i+= 8) 
            {
                dst[RIDX(x, i, dim)] = src[RIDX(i, j, dim)]; 
                if (i + 1 < dim) dst[RIDX(x, i+1, dim)] = src[RIDX(i+1, j, dim)];
                if (i + 2 < dim) dst[RIDX(x, i+2, dim)] = src[RIDX(i+2, j, dim)];
                if (i + 3 < dim) dst[RIDX(x, i+3, dim)] = src[RIDX(i+3, j, dim)];
                if (i + 4 < dim) dst[RIDX(x, i+4, dim)] = src[RIDX(i+4, j, dim)];
                if (i + 5 < dim) dst[RIDX(x, i+5, dim)] = src[RIDX(i+5, j, dim)];
                if (i + 6 < dim) dst[RIDX(x, i+6, dim)] = src[RIDX(i+6, j, dim)];
                if (i + 7 < dim) dst[RIDX(x, i+7, dim)] = src[RIDX(i+7, j, dim)];
            }
        }
}


char my_rotate5_descr[] = "my_rotate5: Unrolling the outer loop";
void my_rotate5(int dim, pixel *src, pixel *dst) 
{
    int i, j, y, x1, x2, x3, x4, x5, x6, x7, j1, j2, j3, j4, j5, j6, j7;

    for (j = 0; j < dim; j += 8) 
    {
	y = dim - 1 - j;
	j1 = j + 1;
	j2 = j + 2;
	j3 = j + 3; 
	j4 = j + 4; 
	j5 = j + 5;
	j6 = j + 6;
	j7 = j + 7;
	x1 = y - 1;
	x2 = y - 2;
	x3 = y - 3;
	x4 = y - 4;
	x5 = y - 5;
	x6 = y - 6;
	x7 = y - 7;

	for (i = 0; i < dim; i++) 
	{
		dst[RIDX(y, i, dim)] = src[RIDX(i, j, dim)];
		dst[RIDX(x1, i, dim)] = src[RIDX(i, j1, dim)];
		dst[RIDX(x2, i, dim)] = src[RIDX(i, j2, dim)];
		dst[RIDX(x3, i, dim)] = src[RIDX(i, j3, dim)];
		dst[RIDX(x4, i, dim)] = src[RIDX(i, j4, dim)];
		dst[RIDX(x5, i, dim)] = src[RIDX(i, j5, dim)];
		dst[RIDX(x6, i, dim)] = src[RIDX(i, j6, dim)];
		dst[RIDX(x7, i, dim)] = src[RIDX(i, j7, dim)];
    	}
}
}

char my_rotate6_descr[] = "my_rotate6: Splitting the image into blocks";
void my_rotate6(int dim, pixel *src, pixel *dst) 
{
    int i, j, jj, ii, B, y, z, x1, x2, x3, x4, x5, x6, x7, j1, j2, j3, j4, j5, j6, j7;
    B = 32; 
    for (jj = 0; jj < dim; jj += B) {
	z = jj + B;
    	for (ii = 0; ii < dim; ii += B) {
		for (j = jj; j < z && j < dim; j += 8) {
			y = dim - 1 - j;
			j1 = j + 1;
			j2 = j + 2;
			j3 = j + 3;
			j4 = j + 4;
			j5 = j + 5;
			j6 = j + 6;
			j7 = j + 7;
			x1 = y - 1;
			x2 = y - 2;
			x3 = y - 3;
			x4 = y - 4;
			x5 = y - 5;
			x6 = y - 6;
			x7 = y - 7;
			for (i = ii; i < ii + B && i < dim; i++) {
				dst[RIDX(y, i, dim)] = src[RIDX(i, j, dim)];
				dst[RIDX(x1, i, dim)] = src[RIDX(i, j1, dim)];
				dst[RIDX(x2, i, dim)] = src[RIDX(i, j2, dim)];
				dst[RIDX(x3, i, dim)] = src[RIDX(i, j3, dim)];
				dst[RIDX(x4, i, dim)] = src[RIDX(i, j4, dim)];
				dst[RIDX(x5, i, dim)] = src[RIDX(i, j5, dim)];
				dst[RIDX(x6, i, dim)] = src[RIDX(i, j6, dim)];
				dst[RIDX(x7, i, dim)] = src[RIDX(i, j7, dim)];

			}
		}
	}
    }
}

char my_rotate7_descr[] = "rotate7: Optimizing with less variables and more unrolling";
void my_rotate7(int dim, pixel *src, pixel *dst) {
	
	int i, j, ii, jj;
	int dim2 = dim - 1;
	for (j = 0; j < dim; j += 16)
		for (i = 0; i < dim; i += 16)
			for (jj = j; jj < j + 16 && jj < dim; jj++)
			{
				int newDim = (dim2 - jj) * dim;
				for (ii = i; ii < i + 16 && ii < dim; ii += 8)
				{
					dst[newDim + ii] = src[ii * dim + jj];
					dst[newDim + ii + 1] = src[(ii + 1) * dim + jj];;
					dst[newDim + ii + 2] = src[(ii + 2) * dim + jj];;
					dst[newDim + ii + 3] = src[(ii + 3) * dim + jj];;
					dst[newDim + ii + 4] = src[(ii + 4) * dim + jj];;
					dst[newDim + ii + 5] = src[(ii + 5) * dim + jj];;
					dst[newDim + ii + 6] = src[(ii + 6) * dim + jj];;
					dst[newDim + ii + 7] = src[(ii + 7) * dim + jj];;
				}
			}	
}

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
/* stride pattern, visualization (we recommend that you draw this for your functions):
    dst         src
    3 7 B F     0 1 2 3
    2 6 A E     4 5 6 7
    1 5 9 D     8 9 A B
    0 4 8 C     C D E F
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
    my_rotate7(dim, src, dst);
}

/*
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function.
 */
void register_rotate_functions() 
{
    // add_rotate_function(&rotate, rotate_descr);
    // add_rotate_function(&my_rotate, my_rotate_descr);
    // add_rotate_function(&my_rotate2, my_rotate2_descr);
    // add_rotate_function(&my_rotate3, my_rotate3_descr);
    // add_rotate_function(&my_rotate4, my_rotate4_descr);
    // add_rotate_function(&my_rotate5, my_rotate5_descr);
    // add_rotate_function(&my_rotate6, my_rotate6_descr);
    add_rotate_function(&my_rotate7, my_rotate7_descr);
    /* ... Register additional test functions here */
}

/******************************************************************************
 * ROTATE_T KERNEL
 *****************************************************************************/

// Your different versions of the rotate_t kernel go here
// (i.e. rotate with multi-threading)

struct thread_data {
	int dim2;
	pixel *src2;
	pixel *dst2;
	int thread_id;
}; 

void *rotate_thread(void *arg) {
	struct thread_data *data;
	data = (struct thread_data *)arg;
	int dim = data->dim2;
	pixel *src = data->src2;
	pixel *dst = data->dst2;
	int thread_id = data->thread_id;
	
	int i, j, ii;
	int dim2 = dim - 1;
	int size_of_block = dim / 8;

	for (i = thread_id * size_of_block; i < (thread_id + 1) * size_of_block; i+= 8)
		for (j = 0; j < dim; j++)
		{
			int newDim = (dim2 - j) * dim;
			for (ii = i; ii < i + 8 && ii < dim; ii++)
			{
				dst[newDim + ii] = src[ii * dim + j];
			}
		}
	return NULL;
}
char my_rotate_t_descr[] = "rotate_t: Multithread try";
void my_rotate_t(int dim, pixel *src, pixel *dst)
{
	if (dim < 512) 
	{
		rotate(dim, src, dst);
		return;
	}
	int i;
	int num_threads = 8;
	pthread_t *thread_array;
	
	struct thread_data thread_data_array[num_threads];

	thread_array = malloc(num_threads * sizeof(pthread_t));

	for (i = 0; i < num_threads; i++) {
		thread_data_array[i].thread_id = i;
		thread_data_array[i].dim2 = dim;
		thread_data_array[i].src2 = src;
		thread_data_array[i].dst2 = dst;
		pthread_create(&thread_array[i], NULL, rotate_thread, (void *)&thread_data_array[i]);
	}	

	for (i = 0; i < num_threads; i++) {
		pthread_join(thread_array[i], NULL);
	}

	free(thread_array);
}
/* 
 * rotate_t - Your current working version of rotate_t
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_t_descr[] = "rotate_t: Current working version";
void rotate_t(int dim, pixel *src, pixel *dst)
{
    my_rotate_t(dim, src, dst);
}

/*********************************************************************
 * register_rotate_t_functions - Register all of your different versions
 *     of the rotate_t kernel with the driver by calling the
 *     add_rotate_t_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_t_functions() 
{
    add_rotate_t_function(&rotate_t, rotate_t_descr);
    add_rotate_t_function(&my_rotate_t, my_rotate_t_descr);
    /* ... Register additional test functions here */
}

/******************************************************************************
 * BLEND KERNEL
 *****************************************************************************/
void my_blend_pixel(pixel* s, pixel* d, pixel* b) {
    float a = ( (float)(s->alpha) ) / USHRT_MAX;
    
    d->red   = (a * s->red  ) + ( (1 - a) * b->red  );
    d->green = (a * s->green) + ( (1 - a) * b->green);
    d->blue  = (a * s->blue) +  ( (1 - a) * b->blue );
    d->alpha = USHRT_MAX; // opaque
}

char my_blend_descr[] = "my_blend: Moving code from blend.c into this file ";
void my_blend(int dim, pixel *src, pixel *dst) // reads global variable `pixel bgc`
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    my_blend_pixel(&src[RIDX(i, j, dim)], &dst[RIDX(i, j, dim)], &bgc); // `blend_pixel` defined in blend.c
}
// Your different versions of the blend kernel go here.

char naive_blend_descr[] = "naive_blend: Naive baseline implementation";
void naive_blend(int dim, pixel *src, pixel *dst) // reads global variable `pixel bgc`
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    blend_pixel(&src[RIDX(i, j, dim)], &dst[RIDX(i, j, dim)], &bgc); // `blend_pixel` defined in blend.c
}

char blend_descr[] = "blend: Current working version";
void blend(int dim, pixel *src, pixel *dst)
{
    naive_blend(dim, src, dst);
}

/*
 * register_blend_functions - Register all of your different versions
 *     of the blend kernel with the driver by calling the
 *     add_blend_function() for each test function.
 */
void register_blend_functions() {
    add_blend_function(&blend, blend_descr);
    add_blend_function(&my_blend, my_blend_descr);
    /* ... Register additional test functions here */
}

/******************************************************************************
 * BLEND_V KERNEL
 *****************************************************************************/

// Your different versions of the blend_v kernel go here
// (i.e. with vectorization, aka. SIMD).

char my_blend_v_descr[] = "Vectors";
void my_blend_v(int dim, pixel *src, pixel *dst)
{
    const __m256 one_vector = _mm256_set1_ps(1.0f);
    const __m256 bgc_vector = _mm256_setr_ps((float)bgc.red, (float)bgc.green, (float)bgc.blue, 0.0F, (float)bgc.red, (float)bgc.green, (float)bgc.blue, 0.0F);
    const __m256i zero_vector = _mm256_setzero_si256();
    const __m256 one_over_ushrt_max = _mm256_set1_ps(1.0F / USHRT_MAX);

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j += 4)
        {
            __m256i pix4 = _mm256_load_si256((__m256i *)&src[RIDX(i, j, dim)]);

            // Convert to floats
            __m256i lower_ints = _mm256_unpacklo_epi16(pix4, zero_vector);
            __m256i upper_ints = _mm256_unpackhi_epi16(pix4, zero_vector);

            __m256 lower_floats = _mm256_cvtepi32_ps(lower_ints);
            __m256 upper_floats = _mm256_cvtepi32_ps(upper_ints);

            // Alpha vectors
            __m256i lower_alpha_ints = _mm256_shuffle_epi32(lower_ints, 0b11111111);
            __m256i upper_alpha_ints = _mm256_shuffle_epi32(upper_ints, 0b11111111);

            __m256 lower_alpha = _mm256_cvtepi32_ps(lower_alpha_ints);
            __m256 upper_alpha = _mm256_cvtepi32_ps(upper_alpha_ints);

            // a = alpha / USHRT_MAX
            __m256 lower_a = _mm256_mul_ps(lower_alpha, one_over_ushrt_max);
            __m256 upper_a = _mm256_mul_ps(upper_alpha, one_over_ushrt_max);

            // (1 - a)
            __m256 lower_one_minus_a = _mm256_sub_ps(one_vector, lower_a);
            __m256 upper_one_minus_a = _mm256_sub_ps(one_vector, upper_a);

            // ((1 - a) * background)
            __m256 lower_bgc = _mm256_mul_ps(lower_one_minus_a, bgc_vector);
            __m256 upper_bgc = _mm256_mul_ps(upper_one_minus_a, bgc_vector);

            // Compute blended values
            __m256 lower_blended = _mm256_fmadd_ps(lower_a, lower_floats, lower_bgc);
            __m256 upper_blended = _mm256_fmadd_ps(upper_a, upper_floats, upper_bgc);

            // Convert blended floats to integers
            __m256i lower_blended_ints = _mm256_cvtps_epi32(lower_blended);
            __m256i upper_blended_ints = _mm256_cvtps_epi32(upper_blended);

            // Pack 32-bit integers into 16-bit unsigned shorts
            __m256i packed = _mm256_packus_epi32(lower_blended_ints, upper_blended_ints);

            // Mask for selecting every fourth element
            __m256i mask = _mm256_set_epi16(-1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0);

            __m256i ushrt_max = _mm256_set1_epi16(USHRT_MAX);

            // Replace every fourth element (alpha values)
            packed = _mm256_blendv_epi8(packed, ushrt_max, mask);

            _mm256_store_si256((__m256i *)&dst[RIDX(i, j, dim)], packed);
        }
    }
}

char blend_v_descr[] = "blend_v: Current working version";
void blend_v(int dim, pixel *src, pixel *dst)
{
    naive_blend(dim, src, dst);
}

/*
 * register_blend_v_functions - Register all of your different versions
 *     of the blend_v kernel with the driver by calling the
 *     add_blend_function() for each test function.
 */
void register_blend_v_functions() {
    add_blend_v_function(&blend_v, blend_v_descr);
	add_blend_v_function(&my_blend_v, my_blend_v_descr);
    /* ... Register additional test functions here */
}

/******************************************************************************
 * SMOOTH KERNEL
 *****************************************************************************/

// Your different versions of the smooth kernel go here

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src); // `avg` defined in smooth.c
}

char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst)
{
  naive_smooth(dim, src, dst);
}

/*
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.
 */

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    /* ... Register additional test functions here */
}
