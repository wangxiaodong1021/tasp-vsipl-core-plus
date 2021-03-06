/* Created by RJudd August 2, 2002 */
/* SPAWARSYSCEN */
/**********************************************************************
// For TASP VSIPL Documentation and Code neither the United States    /
// Government, the United States Navy, nor any of their employees,    /
// makes any warranty, express or implied, including the warranties   /
// of merchantability and fitness for a particular purpose, or        /
// assumes any legal liability or responsibility for the accuracy,    /
// completeness, or usefulness of any information, apparatus,         /
// product, or process disclosed, or represents that its use would    /
// not infringe privately owned rights                                /
**********************************************************************/
/* $Id: VI_fft_building_blocks_f.h,v 2.2 2006/04/27 01:58:00 judd Exp $ */
#ifndef VI_FFT_BUILDING_BLOCKS_F_H
#define VI_FFT_BUILDING_BLOCKS_F_H 1

#define VI_ft_f_2PI 6.2831853071795864769252866
#define VI_ft_f_PI  3.1415926535897932384626433

/*========================================================*/
/* dft */
/* Adapted For VSIP from Fortan example */
/* Burrus and Parks,"DFT/FFT and Convolution Algorithms" */
/******************************************************************/

static
void 
VI_dft_f(void *tfft) {
  vsip_fft_f *fft = (vsip_fft_f*) tfft;
  vsip_cvview_f *a = fft->x;
  vsip_scalar_vi i = 0, j = 0, k = 0;
  vsip_scalar_vi N = (vsip_scalar_vi)a->length;
  vsip_cvview_f *b = fft->wt,
                *c   = fft->temp;
  vsip_stride cast = a->block->cstride;
  vsip_stride cbst = b->block->cstride;
  vsip_stride ccst = c->block->cstride;
  /* register */ 
  vsip_stride ast = (cast * a->stride),
              bst = (cbst * fft->N / N),
              cst = (ccst * c->stride);
  vsip_scalar_f *aprp = (vsip_scalar_f*) ((a->block->R->array) + cast * a->offset),
                *apr,
                *bpr  = (vsip_scalar_f*) ((b->block->R->array) + cbst * b->offset),
                *tbpr,
                *cpr = (vsip_scalar_f*) ((c->block->R->array) + ccst * c->offset);
  vsip_scalar_f *apip = (vsip_scalar_f*) ((a->block->I->array) + cast * a->offset),
                *api,
                *bpi  = (vsip_scalar_f*) ((b->block->I->array) + cbst * b->offset),
                *tbpi,
                *cpi = (vsip_scalar_f*) ((c->block->I->array) + cbst * c->offset);
  b->length = N;
  c->length = N;
  for(j=0; j<N; j++){
    apr = aprp; api = apip;
    *cpr = 0; *cpi = 0;
    *cpr += (*apr * *bpr - *api * *bpi);
    *cpi += (*api * *bpr + *apr * *bpi);
    k = 0;
    apr += ast; api += ast;
    for(i=1; i<N; i++){
      k += j;
      if(k > N-1) k -= N;
      tbpr = bpr + k * bst;
      tbpi = bpi + k * bst;
      *cpr += (*apr * *tbpr - *api * *tbpi); 
      *cpi += (*api * *tbpr + *apr * *tbpi);;
      apr += ast; api += ast;
    }
    cpr += cst; cpi += cst;
  }
  vsip_cvcopy_f_f(c,a);
  return;
} 

/*========================================================*/
/* Created by RJudd August 27, 1998*/
/* SPAWARSYSCEN D881 */

static
void 
VI_fft2_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i; 
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0, a1; 
   vsip_cscalar_f T1;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i;
/*   T1 = vsip_csub_f(a0,a1); */
   T1.r = a0.r - a1.r; T1.i = a0.i - a1.i;
   *xp_r = T1.r; 
   *xp_i = T1.i; 
    xp_r -= xpst; xp_i -= xpst;
   *xp_r = a0.r + a1.r; 
   *xp_i = a0.i + a1.i;
   return;
}

/*========================================================*/
/* Created by RJudd August 27, 1998*/
/* SPAWARSYSCEN D881 */
/* base 2 building block */
/* same as fft2, here for convenience */

static
void 
VI_ifft2_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i; 
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0, a1; 
   vsip_cscalar_f T1;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i;
/*   T1 = vsip_csub_f(a0,a1); */
   T1.r = a0.r - a1.r; T1.i = a0.i - a1.i;
   *xp_r = T1.r; 
   *xp_i = T1.i; 
    xp_r -= xpst; xp_i -= xpst;
   *xp_r = a0.r + a1.r; 
   *xp_i = a0.i + a1.i;
   return;
}

/*========================================================*/
/* Created by RJudd August 27, 1998*/
/* SPAWARSYSCEN D881 */
/* Base 3 building Block */
#define C3PC  (vsip_scalar_f)-1.5
#define C3PS  (vsip_scalar_f).866025403784

static
void 
VI_fft3_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i; 
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,b0,d0;
   vsip_cscalar_f a1,b1,c1;
   vsip_cscalar_f a2,b2,c2;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst; 
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst; 
   a2.r = *xp_r; a2.i = *xp_i;
  /* b1 = vsip_cadd_f(a1,a2);*/
   b1.r = a1.r + a2.r; b1.i = a1.i + a2.i;
  /* b2 = vsip_csub_f(a1,a2); */
   b2.r = a1.r - a2.r; b2.i = a1.i - a2.i;
  /* b0 = vsip_cadd_f(a0,b1); */
   b0.r = a0.r + b1.r; b0.i = a0.i + b1.i;
  /* c1 = vsip_cmplx_f(C3PC * vsip_real_f(b1),C3PC * vsip_imag_f(b1)); */
   c1.r = C3PC * b1.r; c1.i = C3PC * b1.i;
  /* c2 = vsip_cmplx_f(C3PS * vsip_imag_f(b2),C3PS * vsip_real_f(b2)); */
   c2.r = C3PS * b2.i;  c2.i = C3PS * b2.r;
  /* d0 = vsip_cadd_f(b0,c1); */
   d0.r = b0.r + c1.r; d0.i = b0.i + c1.i;
   *xp_r = d0.r - c2.r; *xp_i = d0.i + c2.i; xp_r -= xpst; xp_i -= xpst;
   *xp_r = d0.r + c2.r; *xp_i = d0.i - c2.i; xp_r -= xpst; xp_i -= xpst;
   *xp_r = b0.r; *xp_i = b0.i;
   return;
}

/*========================================================*/
/* Created by RJudd August 27, 1998*/
/* SPAWARSYSCEN D881 */
/* Base 3 building block */
/* Based on Winograd 3-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */

static
void 
VI_ifft3_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i;
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,b0,d0;
   vsip_cscalar_f a1,b1,c1;
   vsip_cscalar_f a2,b2,c2;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i;
  /* b1 = vsip_cadd_f(a1,a2); */
     b1.r = a1.r + a2.r; b1.i = a1.i + a2.i;
  /* b2 = vsip_csub_f(a1,a2); */
     b2.r = a1.r - a2.r; b2.i = a1.i - a2.i;
  /* b0 = vsip_cadd_f(a0,b1); */
     b0.r = a0.r + b1.r; b0.i = a0.i + b1.i;
  /* c1 = vsip_cmplx_f(C3PC * vsip_real_f(b1),C3PC * vsip_imag_f(b1)); */
     c1.r = C3PC * b1.r; c1.i = C3PC * b1.i;
  /* c2 = vsip_cmplx_f(-C3PS * vsip_imag_f(b2),-C3PS * vsip_real_f(b2)); */
     c2.r = -C3PS * b2.i; c2.i = -C3PS * b2.r;
  /* d0 = vsip_cadd_f(b0,c1); */
     d0.r = b0.r + c1.r; d0.i = b0.i + c1.i;
   /*vsip_cvput_f(y,2,vsip_cmplx_f(vsip_real_f(d0)-vsip_real_f(c2),
                                 vsip_imag_f(d0)+vsip_imag_f(c2)));*/
   *xp_r = d0.r - c2.r; *xp_i = d0.i + c2.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,1,vsip_cmplx_f(vsip_real_f(d0)+vsip_real_f(c2),
                                 vsip_imag_f(d0)-vsip_imag_f(c2)));*/
   *xp_r = d0.r + c2.r; *xp_i = d0.i - c2.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,0,b0);*/
   *xp_r = b0.r; *xp_i = b0.i;

   return;
}
/*========================================================*/
/* Created by RJudd August 27, 1998*/
/* SPAWARSYSCEN D881 */
/* Base 4 building block */

static
void 
VI_fft4_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i;
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3;
   vsip_cscalar_f b0,b1,b2,b3;

   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i;

   /*b0 = vsip_cadd_f(a0,a2);*/
   b0.r = a0.r + a2.r; b0.i = a0.i + a2.i;
   /*b1 = vsip_csub_f(a0,a2);*/
   b1.r = a0.r - a2.r; b1.i = a0.i - a2.i;
   /*b2 = vsip_cadd_f(a1,a3);*/
   b2.r = a1.r + a3.r; b2.i = a1.i + a3.i;
   /*b3 = vsip_csub_f(a1,a3);*/
   b3.r = a1.r - a3.r; b3.i = a1.i - a3.i;
   *xp_r = b1.r - b3.i; *xp_i = b1.i + b3.r; xp_r -= xpst; xp_i -= xpst;
   *xp_r = b0.r - b2.r; *xp_i = b0.i - b2.i; xp_r -= xpst; xp_i -= xpst;
   *xp_r = b1.r + b3.i; *xp_i = b1.i - b3.r; xp_r -= xpst; xp_i -= xpst;
   *xp_r = b0.r + b2.r; *xp_i = b0.i + b2.i;
   return;
}

/*========================================================*/
/* Created by RJudd August 27, 1998*/
/* SPAWARSYSCEN D881 */
/* 4 point building block */
/* Based on 4-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */ 
/* Winthrop Smith, Joanne Smith */

static
void 
VI_ifft4_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i;
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3;
   vsip_cscalar_f b0,b1,b2,b3;

   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i;

   /*b0 = vsip_cadd_f(a0,a2);*/
   b0.r = a0.r + a2.r; b0.i = a0.i + a2.i;
   /*b1 = vsip_csub_f(a0,a2);*/
   b1.r = a0.r - a2.r; b1.i = a0.i - a2.i;
   /*b2 = vsip_cadd_f(a1,a3);*/
   b2.r = a1.r + a3.r; b2.i = a1.i + a3.i;
   /*b3 = vsip_csub_f(a1,a3);*/
   b3.r = a1.r - a3.r; b3.i = a1.i - a3.i;
   /*vsip_cvput_f(y,3,vsip_cmplx_f(
                    vsip_real_f(b1) + vsip_imag_f(b3),
                    vsip_imag_f(b1) - vsip_real_f(b3)));*/
   *xp_r = b1.r + b3.i; *xp_i = b1.i - b3.r; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,2,vsip_csub_f(b0,b2));*/
   *xp_r = b0.r - b2.r; *xp_i = b0.i - b2.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,1,vsip_cmplx_f(
                    vsip_real_f(b1) - vsip_imag_f(b3),
                    vsip_imag_f(b1) + vsip_real_f(b3)));*/
   *xp_r = b1.r - b3.i; *xp_i = b1.i + b3.r; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,0,vsip_cadd_f(b0,b2));*/
   *xp_r = b0.r + b2.r; *xp_i = b0.i + b2.i;

   return;
}

/*========================================================*/
/* Created By RJudd August 26, 1998 */
/* SPAWARSYSCEN D881 */
/* Base 5 building block */
/* Based on Winograd 5-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */
#define C5c1 ((vsip_scalar_f)-1.25)
#define C5c2 ((vsip_scalar_f)+0.55901699437495)
#define C5c3 ((vsip_scalar_f)+0.58778525229247)
#define C5c4 ((vsip_scalar_f)+1.53884176858763)
#define C5c5 ((vsip_scalar_f)+0.36327126400268)

static
void 
VI_fft5_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i; 
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3,a4 ;
   vsip_cscalar_f b1,b2,b3,b4;
   vsip_cscalar_f c1,c3,c5;
   vsip_cscalar_f d0,d1,d2,d4;
   vsip_cscalar_f e1,e3,e5;
   vsip_cscalar_f f1,f2,f3,f4;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a4.r = *xp_r; a4.i = *xp_i;

/* step 1 */
/*   b1 = vsip_cadd_f(a1,a4); */
     b1.r = a1.r + a4.r; b1.i = a1.i + a4.i;
/*   b2 = vsip_csub_f(a1,a4); */
     b2.r = a1.r - a4.r; b2.i = a1.i - a4.i;
/*   b3 = vsip_cadd_f(a2,a3); */
     b3.r = a2.r + a3.r; b3.i = a2.i + a3.i;
/*   b4 = vsip_csub_f(a3,a2); */
     b4.r = a3.r - a2.r; b4.i = a3.i - a2.i;
/*   c1 = vsip_cadd_f(b1,b3); */
     c1.r = b1.r + b3.r; c1.i = b1.i + b3.i;
/*   c3 = vsip_csub_f(b1,b3); */
     c3.r = b1.r - b3.r; c3.i = b1.i - b3.i;
/*   c5 = vsip_cadd_f(b2,b4); */
     c5.r = b2.r + b4.r; c5.i = b2.i + b4.i;
/*   d0 = vsip_cadd_f(c1,a0); */
     d0.r = c1.r + a0.r; d0.i = c1.i + a0.i;
/* step 2 */
/*   d1 = vsip_cmplx_f( vsip_real_f(c1) * C5c1, vsip_imag_f(c1) * C5c1); */
     d1.r = c1.r * C5c1; d1.i = c1.i * C5c1;
/*   e3 = vsip_cmplx_f( vsip_real_f(c3) * C5c2, vsip_imag_f(c3) * C5c2); */
     e3.r = c3.r * C5c2; e3.i = c3.i * C5c2;
/*   e5 = vsip_cmplx_f( vsip_real_f(c5) * C5c3, vsip_imag_f(c5) * C5c3); */
     e5.r = c5.r * C5c3; e5.i = c5.i * C5c3;
/*   d2 = vsip_cmplx_f( vsip_imag_f(b2) * C5c4,-vsip_real_f(b2) * C5c4); */
     d2.r = b2.i * C5c4; d2.i = -(b2.r * C5c4);
/*   d4 = vsip_cmplx_f(-vsip_imag_f(b4) * C5c5, vsip_real_f(b4) * C5c5); */
     d4.r = -(b4.i * C5c5); d4.i = b4.r * C5c5;
/* step 3 */
/*   e1 = vsip_cadd_f(d1,d0); */
     e1.r = d1.r + d0.r; e1.i = d1.i + d0.i;
/*   f1 = vsip_cadd_f(e1,e3); */
     f1.r = e1.r + e3.r; f1.i = e1.i + e3.i;
/*   f2 = vsip_cmplx_f(vsip_real_f(d2) - vsip_imag_f(e5),
                       vsip_imag_f(d2) + vsip_real_f(e5)); */
     f2.r = d2.r - e5.i; f2.i = d2.i + e5.r;
/*   f3 = vsip_csub_f(e1,e3); */
     f3.r = e1.r - e3.r; f3.i = e1.i - e3.i;
/*   f4 = vsip_cmplx_f(vsip_real_f(d4) - vsip_imag_f(e5),
                     vsip_imag_f(d4) + vsip_real_f(e5)); */
     f4.r = d4.r - e5.i; f4.i = d4.i + e5.r;
/* step 4 */
   /*vsip_cvput_f(y,4,vsip_csub_f(f1,f2));*/
   *xp_r = f1.r - f2.r; *xp_i = f1.i - f2.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,3,vsip_cadd_f(f3,f4)); */
   *xp_r = f3.r + f4.r; *xp_i = f3.i + f4.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,2,vsip_csub_f(f3,f4));*/
   *xp_r = f3.r - f4.r; *xp_i = f3.i - f4.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,1,vsip_cadd_f(f1,f2)); */
   *xp_r = f1.r + f2.r; *xp_i = f1.i + f2.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,0,d0); */
   *xp_r = d0.r; *xp_i = d0.i;
   return;
}

/*========================================================*/
/* Created By RJudd August 26, 1998 */
/* SPAWARSYSCEN D881 */
/* 5 point building block */
/* Based on Winograd 5-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */
#define iC5c1 ((vsip_scalar_f)-1.25)
#define iC5c2 ((vsip_scalar_f)+0.55901699437495)
#define iC5c3 ((vsip_scalar_f)-0.58778525229247)
#define iC5c4 ((vsip_scalar_f)-1.53884176858763)
#define iC5c5 ((vsip_scalar_f)-0.36327126400268)

static
void 
VI_ifft5_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i;
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3,a4 ;
   vsip_cscalar_f b1,b2,b3,b4;
   vsip_cscalar_f c1,c3,c5;
   vsip_cscalar_f d0,d1,d2,d4;
   vsip_cscalar_f e1,e3,e5;
   vsip_cscalar_f f1,f2,f3,f4;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a4.r = *xp_r; a4.i = *xp_i;
/* step 1 */
/*   b1 = vsip_cadd_f(a1,a4); */
     b1.r = a1.r + a4.r; b1.i = a1.i + a4.i; 
/*   b2 = vsip_csub_f(a1,a4); */  
     b2.r = a1.r - a4.r; b2.i = a1.i - a4.i;
/*   b3 = vsip_cadd_f(a2,a3); */  
     b3.r = a2.r + a3.r; b3.i = a2.i + a3.i;
/*   b4 = vsip_csub_f(a3,a2); */  
     b4.r = a3.r - a2.r; b4.i = a3.i - a2.i;
/*   c1 = vsip_cadd_f(b1,b3); */  
     c1.r = b1.r + b3.r; c1.i = b1.i + b3.i;
/*   c3 = vsip_csub_f(b1,b3); */  
     c3.r = b1.r - b3.r; c3.i = b1.i - b3.i; 
/*   c5 = vsip_cadd_f(b2,b4); */  
     c5.r = b2.r + b4.r; c5.i = b2.i + b4.i; 
/*   d0 = vsip_cadd_f(c1,a0); */  
     d0.r = c1.r + a0.r; d0.i = c1.i + a0.i;
/* step 2 */
/*   d1 = vsip_cmplx_f( vsip_real_f(c1) * iC5c1, vsip_imag_f(c1) * iC5c1); */
     d1.r = c1.r * iC5c1; d1.i = c1.i * iC5c1; 
/*   e3 = vsip_cmplx_f( vsip_real_f(c3) * iC5c2, vsip_imag_f(c3) * iC5c2); */  
     e3.r = c3.r * iC5c2; e3.i = c3.i * iC5c2; 
/*   e5 = vsip_cmplx_f( vsip_real_f(c5) * iC5c3, vsip_imag_f(c5) * iC5c3); */  
     e5.r = c5.r * iC5c3; e5.i = c5.i * iC5c3; 
/*   d2 = vsip_cmplx_f( vsip_imag_f(b2) * iC5c4,-vsip_real_f(b2) * iC5c4); */  
     d2.r = b2.i * iC5c4; d2.i = -(b2.r * iC5c4);
/*   d4 = vsip_cmplx_f(-vsip_imag_f(b4) * iC5c5, vsip_real_f(b4) * iC5c5); */  
     d4.r = -(b4.i * iC5c5); d4.i = b4.r * iC5c5; 
/* step 3 */
/*   e1 = vsip_cadd_f(d1,d0); */
     e1.r = d1.r + d0.r; e1.i = d1.i + d0.i; 
/*   f1 = vsip_cadd_f(e1,e3);*/
     f1.r = e1.r + e3.r; f1.i = e1.i + e3.i; 
/*   f2 = vsip_cmplx_f(vsip_real_f(d2) - vsip_imag_f(e5),
                     vsip_imag_f(d2) + vsip_real_f(e5));*/
     f2.r = d2.r - e5.i; f2.i = d2.i + e5.r; 
/*   f3 = vsip_csub_f(e1,e3);*/
     f3.r = e1.r - e3.r; f3.i = e1.i - e3.i; 
/*   f4 = vsip_cmplx_f(vsip_real_f(d4) - vsip_imag_f(e5),
                     vsip_imag_f(d4) + vsip_real_f(e5));*/
     f4.r = d4.r - e5.i; f4.i = d4.i + e5.r;
/* step 4 */
   /*vsip_cvput_f(y,4,vsip_csub_f(f1,f2));*/
   *xp_r = f1.r - f2.r; *xp_i = f1.i - f2.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,3,vsip_cadd_f(f3,f4)); */
   *xp_r = f3.r + f4.r; *xp_i = f3.i + f4.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,2,vsip_csub_f(f3,f4));*/
   *xp_r = f3.r - f4.r; *xp_i = f3.i - f4.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,1,vsip_cadd_f(f1,f2)); */
   *xp_r = f1.r + f2.r; *xp_i = f1.i + f2.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,0,d0); */
   *xp_r = d0.r; *xp_i = d0.i;

   return;
}

/*========================================================*/
/* Created By RJudd August 26, 1998 */
/* SPAWARSYSCEN D881 */
/* Base 7 building block */
/* Based on Winograd 7-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */
#define C7c1 ((vsip_scalar_f)-1.16666666666667) /*C7c1=-1 + (cos(2 * pi/7) + cos(4 * pi/7) + cos(6 * pi/7))/3*/
#define C7c2 ((vsip_scalar_f)0.79015646852540) /*C7c2=(2 * cos(2 * pi/7) - cos(4 * pi/7) - cos(6 * pi/7))/3*/
#define C7c3 ((vsip_scalar_f)0.05585426728965) /*C7c3=(cos(2 * pi/7) - 2 * cos(4 * pi/7) + cos(6 * pi/7))/3*/
#define C7c4 ((vsip_scalar_f)0.73430220123575) /*C7c4=(cos(2 * pi/7) + cos(4 * pi/7) - 2 * cos(6 * pi/7))/3*/
#define C7c5 ((vsip_scalar_f)0.44095855184410) /*C7c5=((sin(2 * pi/7) + sin(4 * pi/7) - sin(6 * pi/7))/3) */
#define C7c6 ((vsip_scalar_f)0.34087293062393) /*C7c6=(2 * sin(2 * pi/7) - sin(4 * pi/7) + sin(6 * pi/7))/3*/
#define C7c7 ((vsip_scalar_f)-0.53396936033773) /*C7c7=( sin(2 * pi/7) - 2 * sin(4 * pi/7) - sin(6 * pi/7))/3*/
#define C7c8 ((vsip_scalar_f)0.87484229096166) /*C7c8=( sin(2 * pi/7) + sin(4 * pi/7) + 2 * sin(6 * pi/7))/3*/

static
void 
VI_fft7_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i; 
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3,a4,a5,a6;
   vsip_cscalar_f b1,b2,b3,b4,b5,b6;
   vsip_cscalar_f c1,c2,c3,c4,c5,c6,c7,c8;
   vsip_cscalar_f d1,d2;
   vsip_cscalar_f e0,e1,e2,e3,e4,e5,e6,e7,e8;
   vsip_cscalar_f f1,f2,f3,f4;
   vsip_cscalar_f g1,g2,g3,g4,g5,g6;
   vsip_cscalar_f h1,h2,h3;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a4.r = *xp_r; a4.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a5.r = *xp_r; a5.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a6.r = *xp_r; a6.i = *xp_i;
/* step 1 */
/*   b1 = vsip_cadd_f(a1,a6); */
     b1.r = a1.r + a6.r; b1.i = a1.i + a6.i;
/*   b2 = vsip_csub_f(a1,a6); */
     b2.r = a1.r - a6.r; b2.i = a1.i - a6.i;
/*   b3 = vsip_cadd_f(a4,a3); */
     b3.r = a4.r + a3.r; b3.i = a4.i + a3.i;
/*   b4 = vsip_csub_f(a4,a3); */
     b4.r = a4.r - a3.r; b4.i = a4.i - a3.i;
/*   b5 = vsip_cadd_f(a2,a5); */
     b5.r = a2.r + a5.r; b5.i = a2.i + a5.i;
/*   b6 = vsip_csub_f(a2,a5); */
     b6.r = a2.r - a5.r; b6.i = a2.i - a5.i;
/* step 2 */
/*   c1 = vsip_cadd_f(b1,b3); */
   c1.r = b1.r + b3.r; c1.i = b1.i + b3.i;
/*   c2 = vsip_csub_f(b3,b5); */
   c2.r = b3.r - b5.r; c2.i = b3.i - b5.i;
/*   c3 = vsip_csub_f(b1,b3); */
   c3.r = b1.r - b3.r; c3.i = b1.i - b3.i;
/*   c4 = vsip_csub_f(b5,b1); */
   c4.r = b5.r - b1.r; c4.i = b5.i - b1.i;
/*   c5 = vsip_cadd_f(b2,b4); */
   c5.r = b2.r + b4.r; c5.i = b2.i + b4.i;
/*   c6 = vsip_csub_f(b2,b4); */
   c6.r = b2.r - b4.r; c6.i = b2.i - b4.i;
/*   c7 = vsip_csub_f(b4,b6); */
   c7.r = b4.r - b6.r; c7.i = b4.i - b6.i;
/*   c8 = vsip_csub_f(b6,b2); */
   c8.r = b6.r - b2.r; c8.i = b6.i - b2.i;
/* step 3 */
/*   d1 = vsip_cadd_f(b5,c1); */
     d1.r = b5.r + c1.r; d1.i = b5.i + c1.i;
/*   d2 = vsip_cadd_f(b6,c5); */
     d2.r = b6.r + c5.r;  d2.i = b6.i + c5.i;
/*   e0 = vsip_cadd_f(a0,d1); */
     e0.r = a0.r + d1.r; e0.i = a0.i + d1.i;
/* step 4 */
/* e1 = vsip_cmplx_f( vsip_real_f(d1) * C7c1, vsip_imag_f(d1) * C7c1); */
   e1.r = d1.r * C7c1; e1.i = d1.i * C7c1;
/* e2 = vsip_cmplx_f( vsip_real_f(c3) * C7c2, vsip_imag_f(c3) * C7c2); */
   e2.r = c3.r * C7c2; e2.i = c3.i * C7c2;
/* e3 = vsip_cmplx_f( vsip_real_f(c2) * C7c3, vsip_imag_f(c2) * C7c3); */
   e3.r = c2.r * C7c3; e3.i = c2.i * C7c3;
/* e4 = vsip_cmplx_f( vsip_real_f(c4) * C7c4, vsip_imag_f(c4) * C7c4); */
   e4.r = c4.r * C7c4; e4.i = c4.i * C7c4;
/* e5 = vsip_cmplx_f(-vsip_imag_f(d2) * C7c5, vsip_real_f(d2) * C7c5); */
   e5.r = -(d2.i * C7c5); e5.i = d2.r * C7c5;
/* e6 = vsip_cmplx_f(-vsip_imag_f(c6) * C7c6, vsip_real_f(c6) * C7c6); */
   e6.r = -(c6.i * C7c6); e6.i = c6.r * C7c6;
/* e7 = vsip_cmplx_f(-vsip_imag_f(c7) * C7c7, vsip_real_f(c7) * C7c7); */
   e7.r = -(c7.i * C7c7); e7.i = c7.r * C7c7;
/* e8 = vsip_cmplx_f(-vsip_imag_f(c8) * C7c8, vsip_real_f(c8) * C7c8); */
   e8.r = -(c8.i * C7c8); e8.i = c8.r * C7c8;
/* step 5 */
/* f1 = vsip_cadd_f(e0,e1); */
   f1.r = e0.r + e1.r; f1.i = e0.i + e1.i;
/* f2 = vsip_cadd_f(e5,e6); */
   f2.r = e5.r + e6.r; f2.i = e5.i + e6.i;
/* f3 = vsip_csub_f(e5,e6); */
   f3.r = e5.r - e6.r; f3.i = e5.i - e6.i;
/* f4 = vsip_csub_f(e5,e7); */
   f4.r = e5.r - e7.r; f4.i = e5.i - e7.i;
/* step 6 */
/* g1 = vsip_cadd_f(f1,e2); */
   g1.r = f1.r + e2.r; g1.i = f1.i + e2.i;
/* g2 = vsip_csub_f(f1,e2); */
   g2.r = f1.r - e2.r; g2.i = f1.i - e2.i;
/* g3 = vsip_csub_f(f1,e3); */
   g3.r = f1.r - e3.r; g3.i = f1.i - e3.i;
/* g4 = vsip_cadd_f(f2,e7); */
   g4.r = f2.r + e7.r; g4.i = f2.i + e7.i;
/* g5 = vsip_csub_f(f3,e8); */
   g5.r = f3.r - e8.r; g5.i = f3.i - e8.i;
/* g6 = vsip_cadd_f(f4,e8); */
   g6.r = f4.r + e8.r; g6.i = f4.i + e8.i;
/* step 7 */
/* h1 = vsip_cadd_f(g1,e3); */
   h1.r = g1.r + e3.r; h1.i = g1.i + e3.i;
/* h2 = vsip_csub_f(g2,e4); */
   h2.r = g2.r - e4.r; h2.i = g2.i - e4.i;
/* h3 = vsip_cadd_f(g3,e4); */
   h3.r = g3.r + e4.r; h3.i = g3.i + e4.i;
/* step 8 */
   /*vsip_cvput_f(y,6,vsip_cadd_f(h1,g4));*/
   *xp_r = h1.r + g4.r; *xp_i = h1.i + g4.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,5,vsip_cadd_f(h2,g5));*/
   *xp_r = h2.r + g5.r; *xp_i = h2.i + g5.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,4,vsip_csub_f(h3,g6));*/
   *xp_r = h3.r - g6.r; *xp_i = h3.i - g6.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,3,vsip_cadd_f(h3,g6));*/
   *xp_r = h3.r + g6.r; *xp_i = h3.i + g6.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,2,vsip_csub_f(h2,g5));*/
   *xp_r = h2.r - g5.r; *xp_i = h2.i - g5.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,1,vsip_csub_f(h1,g4));*/
   *xp_r = h1.r - g4.r; *xp_i = h1.i - g4.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,0,e0);*/
   *xp_r = e0.r; *xp_i = e0.i;
   return;
}

/*========================================================*/
/* Created By RJudd August 26, 1998 */
/* SPAWARSYSCEN D881 */
/* 7-point building block */
/* Based on Winograd 7-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */
#define iC7c1 ((vsip_scalar_f)-1.16666666666667) /*C7c1=-1 + (cos(2 * pi/7) + cos(4 * pi/7) + cos(6 * pi/7))/3*/
#define iC7c2 ((vsip_scalar_f)0.79015646852540) /*C7c2=(2 * cos(2 * pi/7) - cos(4 * pi/7) - cos(6 * pi/7))/3*/
#define iC7c3 ((vsip_scalar_f)0.05585426728965) /*C7c3=(cos(2 * pi/7) - 2 * cos(4 * pi/7) + cos(6 * pi/7))/3*/
#define iC7c4 ((vsip_scalar_f)0.73430220123575) /*C7c4=(cos(2 * pi/7) + cos(4 * pi/7) - 2 * cos(6 * pi/7))/3*/
#define iC7c5 ((vsip_scalar_f)-0.44095855184410) /*C7c5=((sin(2 * pi/7) + sin(4 * pi/7) - sin(6 * pi/7))/3) */
#define iC7c6 ((vsip_scalar_f)-0.34087293062393) /*C7c6=(2 * sin(2 * pi/7) - sin(4 * pi/7) + sin(6 * pi/7))/3*/
#define iC7c7 ((vsip_scalar_f)+0.53396936033773) /*C7c7=( sin(2 * pi/7) - 2 * sin(4 * pi/7) - sin(6 * pi/7))/3*/
#define iC7c8 ((vsip_scalar_f)-0.87484229096166)  /*C7c8=( sin(2 * pi/7) + sin(4 * pi/7) + 2 * sin(6 * pi/7))/3*/

static
void 
VI_ifft7_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i;
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3,a4,a5,a6;
   vsip_cscalar_f b1,b2,b3,b4,b5,b6;
   vsip_cscalar_f c1,c2,c3,c4,c5,c6,c7,c8;
   vsip_cscalar_f d1,d2;
   vsip_cscalar_f e0,e1,e2,e3,e4,e5,e6,e7,e8;
   vsip_cscalar_f f1,f2,f3,f4;
   vsip_cscalar_f g1,g2,g3,g4,g5,g6;
   vsip_cscalar_f h1,h2,h3;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a4.r = *xp_r; a4.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a5.r = *xp_r; a5.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a6.r = *xp_r; a6.i = *xp_i;
/* step 1 */
/*   b1 = vsip_cadd_f(a1,a6); */
     b1.r = a1.r + a6.r; b1.i = a1.i + a6.i; 
/*   b2 = vsip_csub_f(a1,a6); */
     b2.r = a1.r - a6.r; b2.i = a1.i - a6.i;
/*   b3 = vsip_cadd_f(a4,a3); */
     b3.r = a4.r + a3.r; b3.i = a4.i + a3.i; 
/*   b4 = vsip_csub_f(a4,a3); */
     b4.r = a4.r - a3.r; b4.i = a4.i - a3.i; 
/*   b5 = vsip_cadd_f(a2,a5); */
     b5.r = a2.r + a5.r; b5.i = a2.i + a5.i; 
/*   b6 = vsip_csub_f(a2,a5); */
     b6.r = a2.r - a5.r; b6.i = a2.i - a5.i; 
/* step 2 */
/*   c1 = vsip_cadd_f(b1,b3); */
   c1.r = b1.r + b3.r; c1.i = b1.i + b3.i; 
/*   c2 = vsip_csub_f(b3,b5); */  
   c2.r = b3.r - b5.r; c2.i = b3.i - b5.i; 
/*   c3 = vsip_csub_f(b1,b3); */  
   c3.r = b1.r - b3.r; c3.i = b1.i - b3.i; 
/*   c4 = vsip_csub_f(b5,b1); */  
   c4.r = b5.r - b1.r; c4.i = b5.i - b1.i; 
/*   c5 = vsip_cadd_f(b2,b4); */  
   c5.r = b2.r + b4.r; c5.i = b2.i + b4.i; 
/*   c6 = vsip_csub_f(b2,b4); */  
   c6.r = b2.r - b4.r; c6.i = b2.i - b4.i; 
/*   c7 = vsip_csub_f(b4,b6); */  
   c7.r = b4.r - b6.r; c7.i = b4.i - b6.i; 
/*   c8 = vsip_csub_f(b6,b2); */
   c8.r = b6.r - b2.r; c8.i = b6.i - b2.i;
/* step 3 */
/*   d1 = vsip_cadd_f(b5,c1); */
     d1.r = b5.r + c1.r; d1.i = b5.i + c1.i;
/*   d2 = vsip_cadd_f(b6,c5); */
     d2.r = b6.r + c5.r;  d2.i = b6.i + c5.i;
/*   e0 = vsip_cadd_f(a0,d1); */
     e0.r = a0.r + d1.r; e0.i = a0.i + d1.i; 
/* step 4 */
/* e1 = vsip_cmplx_f( vsip_real_f(d1) * iC7c1, vsip_imag_f(d1) * iC7c1); */
   e1.r = d1.r * iC7c1; e1.i = d1.i * iC7c1;
/* e2 = vsip_cmplx_f( vsip_real_f(c3) * iC7c2, vsip_imag_f(c3) * iC7c2); */
   e2.r = c3.r * iC7c2; e2.i = c3.i * iC7c2;
/* e3 = vsip_cmplx_f( vsip_real_f(c2) * iC7c3, vsip_imag_f(c2) * iC7c3); */
   e3.r = c2.r * iC7c3; e3.i = c2.i * iC7c3;
/* e4 = vsip_cmplx_f( vsip_real_f(c4) * iC7c4, vsip_imag_f(c4) * iC7c4); */
   e4.r = c4.r * iC7c4; e4.i = c4.i * iC7c4;
/* e5 = vsip_cmplx_f(-vsip_imag_f(d2) * iC7c5, vsip_real_f(d2) * iC7c5); */
   e5.r = -(d2.i * iC7c5); e5.i = d2.r * iC7c5;
/* e6 = vsip_cmplx_f(-vsip_imag_f(c6) * iC7c6, vsip_real_f(c6) * iC7c6); */
   e6.r = -(c6.i * iC7c6); e6.i = c6.r * iC7c6;
/* e7 = vsip_cmplx_f(-vsip_imag_f(c7) * iC7c7, vsip_real_f(c7) * iC7c7); */
   e7.r = -(c7.i * iC7c7); e7.i = c7.r * iC7c7;
/* e8 = vsip_cmplx_f(-vsip_imag_f(c8) * iC7c8, vsip_real_f(c8) * iC7c8); */
   e8.r = -(c8.i * iC7c8); e8.i = c8.r * iC7c8;
/* step 5 */
/* f1 = vsip_cadd_f(e0,e1); */
   f1.r = e0.r + e1.r; f1.i = e0.i + e1.i;
/* f2 = vsip_cadd_f(e5,e6); */
   f2.r = e5.r + e6.r; f2.i = e5.i + e6.i;
/* f3 = vsip_csub_f(e5,e6); */
   f3.r = e5.r - e6.r; f3.i = e5.i - e6.i;
/* f4 = vsip_csub_f(e5,e7); */
   f4.r = e5.r - e7.r; f4.i = e5.i - e7.i;
/* step 6 */
/* g1 = vsip_cadd_f(f1,e2); */
   g1.r = f1.r + e2.r; g1.i = f1.i + e2.i;
/* g2 = vsip_csub_f(f1,e2); */
   g2.r = f1.r - e2.r; g2.i = f1.i - e2.i;
/* g3 = vsip_csub_f(f1,e3); */
   g3.r = f1.r - e3.r; g3.i = f1.i - e3.i;
/* g4 = vsip_cadd_f(f2,e7); */
   g4.r = f2.r + e7.r; g4.i = f2.i + e7.i;
/* g5 = vsip_csub_f(f3,e8); */
   g5.r = f3.r - e8.r; g5.i = f3.i - e8.i;
/* g6 = vsip_cadd_f(f4,e8); */
   g6.r = f4.r + e8.r; g6.i = f4.i + e8.i;
/* step 7 */
/* h1 = vsip_cadd_f(g1,e3); */
   h1.r = g1.r + e3.r; h1.i = g1.i + e3.i;
/* h2 = vsip_csub_f(g2,e4); */
   h2.r = g2.r - e4.r; h2.i = g2.i - e4.i;
/* h3 = vsip_cadd_f(g3,e4); */
   h3.r = g3.r + e4.r; h3.i = g3.i + e4.i;
/* step 8 */
   /*vsip_cvput_f(y,6,vsip_cadd_f(h1,g4));*/
   *xp_r = h1.r + g4.r; *xp_i = h1.i + g4.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,5,vsip_cadd_f(h2,g5));*/
   *xp_r = h2.r + g5.r; *xp_i = h2.i + g5.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,4,vsip_csub_f(h3,g6));*/
   *xp_r = h3.r - g6.r; *xp_i = h3.i - g6.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,3,vsip_cadd_f(h3,g6));*/
   *xp_r = h3.r + g6.r; *xp_i = h3.i + g6.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,2,vsip_csub_f(h2,g5));*/
   *xp_r = h2.r - g5.r; *xp_i = h2.i - g5.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,1,vsip_csub_f(h1,g4));*/
   *xp_r = h1.r - g4.r; *xp_i = h1.i - g4.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,0,e0);*/
   *xp_r = e0.r; *xp_i = e0.i;

   return;
}

/*========================================================*/
/* Created By RJudd August 26, 1998 */
/* SPAWARSYSCEN D881 */
/* Base 8 building block */
/* Based on Winograd 8-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */

#define C8c1 ((vsip_scalar_f)0.70710678118655) /*cos(pi/4)*/

static
void 
VI_fft8_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i; 
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3,a4,a5,a6,a7;
   vsip_cscalar_f b0,b1,b2,b3,b4,b5,b6,b7;
   vsip_cscalar_f c0,c1,c2,c3,c4,c5;
   vsip_cscalar_f d0,d1,d2,d3,d4,d5,d6,d7;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a4.r = *xp_r; a4.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a5.r = *xp_r; a5.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a6.r = *xp_r; a6.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a7.r = *xp_r; a7.i = *xp_i;
/* step 1 */
/*   b0 = vsip_cadd_f(a0,a4); */
   b0.r = a0.r + a4.r; b0.i = a0.i + a4.i;
/*   b1 = vsip_csub_f(a0,a4); */
   b1.r = a0.r - a4.r; b1.i = a0.i - a4.i;
/*   b2 = vsip_cadd_f(a1,a5);*/
   b2.r = a1.r + a5.r; b2.i = a1.i + a5.i;
/*   b3 = vsip_csub_f(a1,a5); */
   b3.r = a1.r - a5.r; b3.i = a1.i - a5.i;
/*   b4 = vsip_cadd_f(a2,a6); */
   b4.r = a2.r + a6.r; b4.i = a2.i + a6.i;
/*   b5 = vsip_csub_f(a2,a6); */
   b5.r = a2.r - a6.r; b5.i = a2.i - a6.i;
/*   b6 = vsip_cadd_f(a3,a7); */
   b6.r = a3.r + a7.r; b6.i = a3.i +a7.i;
/*   b7 = vsip_csub_f(a3,a7); */
   b7.r = a3.r - a7.r; b7.i = a3.i - a7.i;
/*   c0 = vsip_cadd_f(b0,b4); */
   c0.r = b0.r + b4.r; c0.i = b0.i + b4.i;
/*   c1 = vsip_csub_f(b0,b4); */
   c1.r = b0.r - b4.r; c1.i = b0.i - b4.i;
/*   c2 = vsip_cadd_f(b2,b6); */
   c2.r = b2.r + b6.r; c2.i = b2.i + b6.i;
/*   c3 = vsip_csub_f(b2,b6); */
   c3.r =b2.r - b6.r; c3.i = b2.i - b6.i;
/*   c4 = vsip_cadd_f(b3,b7); */
   c4.r = b3.r + b7.r; c4.i = b3.i + b7.i;
/*   c5 = vsip_csub_f(b3,b7); */
   c5.r = b3.r - b7.r; c5.i = b3.i - b7.i;
/* step 2 */
/*   c4 = vsip_cmplx_f( vsip_real_f(c4) * C8c1, vsip_imag_f(c4) * C8c1); */
   c4.r = c4.r * C8c1; c4.i =  c4.i * C8c1; 
/*   c5 = vsip_cmplx_f( vsip_real_f(c5) * C8c1, vsip_imag_f(c5) * C8c1); */
   c5.r = c5.r * C8c1; c5.i = c5.i * C8c1; 
/* step 3 */
/*   d0 = vsip_cadd_f(c0,c2); */
   d0.r = c0.r + c2.r; d0.i = c0.i + c2.i;
/*   d4 = vsip_csub_f(c0,c2); */
   d4.r = c0.r - c2.r; d4.i = c0.i - c2.i;
/*   d2 = vsip_cmplx_f(vsip_real_f(c1) + vsip_imag_f(c3),vsip_imag_f(c1) - vsip_real_f(c3));*/
   d2.r = c1.r + c3.i; d2.i = c1.i - c3.r;
/*   d6 = vsip_cmplx_f(vsip_real_f(c1) - vsip_imag_f(c3),vsip_imag_f(c1) + vsip_real_f(c3));*/
   d6.r = c1.r - c3.i; d6.i = c1.i + c3.r;
/*   d1 = vsip_cadd_f(b1,c5); */
   d1.r = b1.r + c5.r; d1.i = b1.i + c5.i;
/*   d5 = vsip_csub_f(b1,c5); */
   d5.r = b1.r - c5.r; d5.i = b1.i - c5.i;
/*   d3 = vsip_cmplx_f(vsip_imag_f(b5) + vsip_imag_f(c4),
                     vsip_real_f(b5) + vsip_real_f(c4)); */
   d3.r = b5.i + c4.i; d3.i = b5.r + c4.r;
/*   d7 = vsip_cmplx_f(vsip_imag_f(c4) - vsip_imag_f(b5),
                     vsip_real_f(b5) - vsip_real_f(c4)); */
   d7.r = c4.i - b5.i; d7.i = b5.r - c4.r;
/* step 4 */
   /*vsip_cvput_f(y,7,vsip_cmplx_f(vsip_real_f(d1) - vsip_real_f(d3),
                                 vsip_imag_f(d1) + vsip_imag_f(d3)));*/
   *xp_r = d1.r - d3.r; *xp_i = d1.i + d3.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,6,d6);*/
   *xp_r = d6.r; *xp_i = d6.i;  xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,5,vsip_cmplx_f(vsip_real_f(d5) - vsip_real_f(d7),
                                 vsip_imag_f(d5) - vsip_imag_f(d7)));*/
   *xp_r = d5.r - d7.r; *xp_i = d5.i - d7.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,4,d4);*/
   *xp_r = d4.r; *xp_i =d4.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,3,vsip_cadd_f(d5,d7));*/
   *xp_r = d5.r + d7.r; *xp_i = d5.i + d7.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,2,d2);*/
   *xp_r = d2.r; *xp_i = d2.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,1,vsip_cmplx_f(vsip_real_f(d1) + vsip_real_f(d3),
                                 vsip_imag_f(d1) - vsip_imag_f(d3)));*/
   *xp_r = d1.r + d3.r; *xp_i = d1.i - d3.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,0,d0);*/
   *xp_r = d0.r; *xp_i = d0.i;

   return;
}

/*========================================================*/
/* Created By RJudd August 26, 1998 */
/* SPAWARSYSCEN D881 */
/* 8-point building block */
/* Based on Winograd 8-point FFT */
/* "Handbook of Real-Time Fast Fourier Transforms */
/* Winthrop Smith, Joanne Smith */

#define C8c1 ((vsip_scalar_f)0.70710678118655) /*cos(pi/4)*/

static
void 
VI_ifft8_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
   vsip_scalar_f *xp_r = fft->xp_r,
                 *xp_i = fft->xp_i;
   vsip_scalar_vi xpst = fft->xpst;
   vsip_cscalar_f a0,a1,a2,a3,a4,a5,a6,a7;
   vsip_cscalar_f b0,b1,b2,b3,b4,b5,b6,b7;
   vsip_cscalar_f c0,c1,c2,c3,c4,c5;
   vsip_cscalar_f d0,d1,d2,d3,d4,d5,d6,d7;
   a0.r = *xp_r; a0.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a1.r = *xp_r; a1.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a2.r = *xp_r; a2.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a3.r = *xp_r; a3.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a4.r = *xp_r; a4.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a5.r = *xp_r; a5.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a6.r = *xp_r; a6.i = *xp_i; xp_r += xpst; xp_i += xpst;
   a7.r = *xp_r; a7.i = *xp_i;
/* step 1 */
/*   b0 = vsip_cadd_f(a0,a4); */
   b0.r = a0.r + a4.r; b0.i = a0.i + a4.i;
/*   b1 = vsip_csub_f(a0,a4); */
   b1.r = a0.r - a4.r; b1.i = a0.i - a4.i;
/*   b2 = vsip_cadd_f(a1,a5);*/
   b2.r = a1.r + a5.r; b2.i = a1.i + a5.i;
/*   b3 = vsip_csub_f(a1,a5); */
   b3.r = a1.r - a5.r; b3.i = a1.i - a5.i;
/*   b4 = vsip_cadd_f(a2,a6); */
   b4.r = a2.r + a6.r; b4.i = a2.i + a6.i;
/*   b5 = vsip_csub_f(a2,a6); */
   b5.r = a2.r - a6.r; b5.i = a2.i - a6.i;
/*   b6 = vsip_cadd_f(a3,a7); */
   b6.r = a3.r + a7.r; b6.i = a3.i +a7.i;
/*   b7 = vsip_csub_f(a3,a7); */
   b7.r = a3.r - a7.r; b7.i = a3.i - a7.i;
/*   c0 = vsip_cadd_f(b0,b4); */
   c0.r = b0.r + b4.r; c0.i = b0.i + b4.i;
/*   c1 = vsip_csub_f(b0,b4); */
   c1.r = b0.r - b4.r; c1.i = b0.i - b4.i;
/*   c2 = vsip_cadd_f(b2,b6); */
   c2.r = b2.r + b6.r; c2.i = b2.i + b6.i;
/*   c3 = vsip_csub_f(b2,b6); */
   c3.r =b2.r - b6.r; c3.i = b2.i - b6.i;
/*   c4 = vsip_cadd_f(b3,b7); */
   c4.r = b3.r + b7.r; c4.i = b3.i + b7.i;
/*   c5 = vsip_csub_f(b3,b7); */
   c5.r = b3.r - b7.r; c5.i = b3.i - b7.i;
/* step 2 */
/*   c4 = vsip_cmplx_f( vsip_real_f(c4) * C8c1, vsip_imag_f(c4) * C8c1); */
   c4.r = c4.r * C8c1; c4.i =  c4.i * C8c1;
/*   c5 = vsip_cmplx_f( vsip_real_f(c5) * C8c1, vsip_imag_f(c5) * C8c1); */
   c5.r = c5.r * C8c1; c5.i = c5.i * C8c1;
/* step 3 */
/*   d0 = vsip_cadd_f(c0,c2); */
   d0.r = c0.r + c2.r; d0.i = c0.i + c2.i;
/*   d4 = vsip_csub_f(c0,c2); */
   d4.r = c0.r - c2.r; d4.i = c0.i - c2.i;
/*   d2 = vsip_cmplx_f(vsip_real_f(c1) - vsip_imag_f(c3),vsip_imag_f(c1) + vsip_real_f(c3));*/
   d2.r = c1.r - c3.i; d2.i = c1.i + c3.r;
/*   d6 = vsip_cmplx_f(vsip_real_f(c1) + vsip_imag_f(c3),vsip_imag_f(c1) - vsip_real_f(c3));*/
   d6.r = c1.r + c3.i; d6.i = c1.i - c3.r;
/*   d1 = vsip_cadd_f(b1,c5); */
   d1.r = b1.r + c5.r; d1.i = b1.i + c5.i;
/*   d5 = vsip_csub_f(b1,c5); */
   d5.r = b1.r - c5.r; d5.i = b1.i - c5.i;
/*   d3 = vsip_cmplx_f(-vsip_imag_f(b5) - vsip_imag_f(c4),
                     -vsip_real_f(b5) - vsip_real_f(c4)); */
      d3.r = -(b5.i + c4.i); d3.i = -(b5.r + c4.r);
/*   d7 = vsip_cmplx_f(-vsip_imag_f(c4) + vsip_imag_f(b5),
                     -vsip_real_f(b5) + vsip_real_f(c4)); */
   d7.r = b5.i - c4.i; d7.i = c4.r - b5.r;
/* step 4 */
   /*vsip_cvput_f(y,7,vsip_cmplx_f(vsip_real_f(d1) - vsip_real_f(d3),
                                 vsip_imag_f(d1) + vsip_imag_f(d3)));*/
   *xp_r = d1.r - d3.r; *xp_i = d1.i + d3.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,6,d6);*/
   *xp_r = d6.r; *xp_i = d6.i;  xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,5,vsip_cmplx_f(vsip_real_f(d5) - vsip_real_f(d7),
                                 vsip_imag_f(d5) - vsip_imag_f(d7)));*/
   *xp_r = d5.r - d7.r; *xp_i = d5.i - d7.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,4,d4);*/
   *xp_r = d4.r; *xp_i =d4.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,3,vsip_cadd_f(d5,d7));*/
   *xp_r = d5.r + d7.r; *xp_i = d5.i + d7.i; xp_r -= xpst; xp_i -= xpst;

   /*vsip_cvput_f(y,2,d2);*/                                             
   *xp_r = d2.r; *xp_i = d2.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,1,vsip_cmplx_f(vsip_real_f(d1) + vsip_real_f(d3),
                                 vsip_imag_f(d1) - vsip_imag_f(d3)));*/
   *xp_r = d1.r + d3.r; *xp_i = d1.i - d3.i; xp_r -= xpst; xp_i -= xpst;
 
   /*vsip_cvput_f(y,0,d0);*/
   *xp_r = d0.r; *xp_i = d0.i;

   return;
}
/* fft_wt.c multiply by twiddle factors */

static
void 
VI_fft_wt_f(void *tfft) {
   vsip_fft_f* fft = (vsip_fft_f*) tfft;
      vsip_cvview_f *a = fft->x;
      vsip_cvview_f *b = fft->wt;
      vsip_cvview_f *r = fft->x;
      vsip_stride cast = a->block->cstride;
      vsip_stride cbst = b->block->cstride;
      vsip_length n = r->length;
      vsip_scalar_f *apr = (vsip_scalar_f *)((a->block->R->array) + cast * a->offset),
                    *bpr = (vsip_scalar_f *)(b->block->R->array),
                    *rpr = apr;
      vsip_scalar_f *api = (vsip_scalar_f *)((a->block->I->array) + cast * a->offset),
                    *bpi = (vsip_scalar_f *)(b->block->I->array),
                    *rpi = api;
      vsip_scalar_f temp;
      /* register */ vsip_stride ast = cast * a->stride, 
                                 bst = cbst * b->stride, 
                                 rst = ast;
      while(n-- > 0){
          temp = *apr * *bpr - *bpi * *api;
          *rpi = *apr * *bpi + *api * *bpr;
          *rpr = temp;
          apr += ast; api += ast; 
          bpr += bst; bpi += bst; 
          rpr += rst; rpi += rst;
      }
   /*vsip_cvmul_f(fft->x, fft->wt, fft->x);*/
   return;
}

static
void 
VI_p0pF_f(void *tfft) {
  vsip_fft_f *fft = (vsip_fft_f*) tfft;
  vsip_scalar_vi p0 = fft->p0[fft->stage];
  vsip_scalar_vi pF = fft->pF[fft->stage];
  vsip_scalar_vi M  = fft->pn[fft->stage];

  int N   = ((fft->stage) == 0) ? fft->N : fft->pF[fft->stage - 1];
  vsip_scalar_vi Ns = N;
  vsip_scalar_vi N1;
  vsip_scalar_vi wtstr = fft->N / N;
  vsip_scalar_vi seg = 1;
  vsip_stride k=0,i=0,j=0,case_pF;
  vsip_stride cxst = fft->x->block->cstride;

  void (*fft_p)(void*) = VI_p0pF_f;

  vsip_cvattr_f xattr;

  fft->x_r = (vsip_scalar_f *)((fft->x->block->R->array) + cxst * fft->x->offset);
  fft->x_i = (vsip_scalar_f *)((fft->x->block->I->array) + cxst * fft->x->offset);
  fft->xst = cxst * fft->x->stride;

  vsip_cvgetattrib_f(fft->x,&xattr);

  fft->stage++;
  case_pF = (pF < 9) ? pF : ((fft->stage) == (fft->length )) ? 101 : 103;

   fft->wt->length = p0;   /* set weight vector length*/
   fft->x->length = p0;     /* set x vector length */

  if(fft->d == -1){ /*forward section */
      switch( p0){ /* fft building block for p0 */
        case 2  : fft_p = VI_fft2_f; break;
        case 3  : fft_p = VI_fft3_f; break;
        case 4  : fft_p = VI_fft4_f; break;
        case 5  : fft_p = VI_fft5_f; break;
        case 7  : fft_p = VI_fft7_f; break;
        case 8  : fft_p = VI_fft8_f; break;
      }
      for(k=0; k<(int)M-1; k++ ) { /* (M-1 segments) *( pF building blocks of length p0)*/
        N1 = Ns;
        Ns = Ns/p0; 
        fft->x->stride = xattr.stride * Ns; /* x stride for segment */
        fft->xpst = Ns * fft->xst;
        fft->wt->stride = 0;
        /*j = 0 */
        fft->x->offset = xattr.offset;
        fft->xp_r = fft->x_r;
        fft->xp_i = fft->x_i;
        fft_p(fft);
        for(i = N1; i < N; i+= N1){
          fft->x->offset = xattr.offset + xattr.stride * i;
          fft->xp_r = fft->x_r + fft->xst * i;
          fft->xp_i = fft->x_i + fft->xst * i;
          fft_p(fft);
          VI_fft_wt_f(fft);
        }
        fft->wt->stride += (seg * wtstr); 
        for(j=1; j<(int)Ns; j++){
          for(i = j; i < N; i+= N1){
            fft->x->offset = xattr.offset + xattr.stride * i;
            fft->xp_r = fft->x_r + fft->xst * i;
            fft->xp_i = fft->x_i + fft->xst * i;
            fft_p(fft);
            VI_fft_wt_f(fft); /*vsip_cvmul_f(fft->x,wt,fft->x);*/
          }
          fft->wt->stride += (seg * wtstr);
        }  
      seg *= p0 ;
      }
      N1 = Ns;
      Ns = Ns/pF; 
      fft->x->stride = xattr.stride * Ns;
      fft->xpst = Ns * fft->xst;
      fft->x->length = pF;
      switch(case_pF){
        case 2  : fft_p = VI_fft2_f; break;
        case 3  : fft_p = VI_fft3_f; break;
        case 4  : fft_p = VI_fft4_f; break;
        case 5  : fft_p = VI_fft5_f; break;
        case 7  : fft_p = VI_fft7_f; break;
        case 8  : fft_p = VI_fft8_f; break;
        case 101: fft_p = VI_dft_f; break;
        case 103: fft_p = VI_p0pF_f; break;
      }
      for(j=0; j<(int)Ns; j++){
        for(i = j; i < N; i+= N1){
          fft->x->offset = xattr.offset + xattr.stride * i;
          fft->xp_r = fft->x_r + fft->xst * i;
          fft->xp_i = fft->x_i + fft->xst * i;
          fft_p(fft);
        }
      }  
    }  /* end forward section */

      else { /*inverse section */
      switch( p0){ /* fft building block for p0 */
        case 2  : fft_p = VI_ifft2_f; break;
        case 3  : fft_p = VI_ifft3_f; break;
        case 4  : fft_p = VI_ifft4_f; break;
        case 5  : fft_p = VI_ifft5_f; break;
        case 7  : fft_p = VI_ifft7_f; break;
        case 8  : fft_p = VI_ifft8_f; break;
      }  
      for(k=0; k<(int)M-1; k++ ) { /* (M-1 segments) *( pF building blocks of length p0)*/
        N1 = Ns;
        Ns = Ns/p0;
        fft->x->stride = xattr.stride * Ns; /* x stride for segment */
        fft->xpst = Ns * fft->xst;
        fft->wt->stride = 0;
        /*j = 0 */ 
        fft->x->offset = xattr.offset;
        fft->xp_r = fft->x_r;
        fft->xp_i = fft->x_i;
        fft_p(fft);
        for(i = N1; i < N; i+= N1){
          fft->x->offset = xattr.offset + xattr.stride * i;
          fft->xp_r = fft->x_r + fft->xst * i;
          fft->xp_i = fft->x_i + fft->xst * i;
          fft_p(fft);
          VI_fft_wt_f(fft);
        }
        fft->wt->stride += (seg * wtstr);
        for(j=1; j<(int)Ns; j++){
          for(i = j; i < N; i+= N1){
            fft->x->offset = xattr.offset + xattr.stride * i;
            fft->xp_r = fft->x_r + fft->xst * i;
            fft->xp_i = fft->x_i + fft->xst * i;
            fft_p(fft);
            VI_fft_wt_f(fft); /*vsip_cvmul_f(fft->x,wt,fft->x);*/
          }
          fft->wt->stride += (seg * wtstr);
        }
      seg *= p0 ;
      }  
      N1 = Ns;
      Ns = Ns/pF;
      fft->x->stride = xattr.stride * Ns;
      fft->xpst = Ns * fft->xst;
      fft->x->length = pF;
      switch(case_pF){
        case 2  : fft_p = VI_ifft2_f; break;
        case 3  : fft_p = VI_ifft3_f; break;
        case 4  : fft_p = VI_ifft4_f; break;
        case 5  : fft_p = VI_ifft5_f; break;
        case 7  : fft_p = VI_ifft7_f; break;
        case 8  : fft_p = VI_ifft8_f; break;
        case 101: fft_p = VI_dft_f; break;
        case 103: fft_p = VI_p0pF_f; break;
      }  
      for(j=0; j<(int)Ns; j++){
        for(i = j; i < N; i+= N1){
          fft->x->offset = xattr.offset + xattr.stride * i;
          fft->xp_r = fft->x_r + fft->xst * i;
          fft->xp_i = fft->x_i + fft->xst * i;
          fft_p(fft);
        }
      }  
    } /* end inverse section */
    vsip_cvputattrib_f(fft->x,&xattr);
    fft->stage--;
    return;
}
#endif
