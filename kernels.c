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
		j1 = j + 1;
		j2 = j + 2;
		j3 = j + 3; 
		j4 = j + 4; 
		j5 = j + 5;
		j6 = j + 6;
		j7 = j + 7;

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

char my_rotate6_descr[] = "my_rotate6: Unrolling the outer loop";
void my_rotate6(int dim, pixel *src, pixel *dst) 
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
		j1 = j + 1;
		j2 = j + 2;
		j3 = j + 3; 
		j4 = j + 4; 
		j5 = j + 5;
		j6 = j + 6;
		j7 = j + 7;

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
    naive_rotate(dim, src, dst);
}

/*
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function.
 */
void register_rotate_functions() 
{
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&my_rotate, my_rotate_descr);
    add_rotate_function(&my_rotate2, my_rotate2_descr);
    add_rotate_function(&my_rotate3, my_rotate3_descr);
    add_rotate_function(&my_rotate4, my_rotate4_descr);
    add_rotate_function(&my_rotate5, my_rotate5_descr);
    add_rotate_function(&my_rotate6, my_rotate6_descr);
    /* ... Register additional test functions here */
}

/******************************************************************************
 * ROTATE_T KERNEL
 *****************************************************************************/

// Your different versions of the rotate_t kernel go here
// (i.e. rotate with multi-threading)

/* 
 * rotate_t - Your current working version of rotate_t
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_t_descr[] = "rotate_t: Current working version";
void rotate_t(int dim, pixel *src, pixel *dst)
{
    naive_rotate(dim, src, dst);
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
    /* ... Register additional test functions here */
}

/******************************************************************************
 * BLEND KERNEL
 *****************************************************************************/

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
    /* ... Register additional test functions here */
}

/******************************************************************************
 * BLEND_V KERNEL
 *****************************************************************************/

// Your different versions of the blend_v kernel go here
// (i.e. with vectorization, aka. SIMD).

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
