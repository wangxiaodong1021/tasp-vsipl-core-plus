/* Created RJudd */
/* SPAWARSYSCEN D857 */
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
/* $Id: mcmaxmgsqval_f.h,v 2.0 2003/02/22 15:23:24 judd Exp $ */
static void mcmaxmgsqval_f(){
   printf("\n*******\nTEST mcmaxmgsqval_f\n\n");
   { vsip_scalar_f data_r[]  = {-1, 2, 0, -5, -6,   3.4, -3.4,  5.6, -.3};
     vsip_scalar_f data_i[]  = {-2, 1, 2,  0, -3,  -3.4, +3.4, -5.6, +.3};
     vsip_cblock_f  *block = vsip_cblockbind_f(data_r,data_i,9,VSIP_MEM_NONE);
     vsip_cmview_f  *a = vsip_cmbind_f(block,0,3,3,1,3);
     vsip_cblock_f  *block2 = vsip_cblockcreate_f(50,VSIP_MEM_NONE);
     vsip_cmview_f  *b = vsip_cmbind_f(block2,49,-2,3,-8,3);

     vsip_scalar_mi index;
     vsip_scalar_mi ind_ans = vsip_matindex(2,1);
     vsip_scalar_f ans = 62.72;
     vsip_scalar_f val;

     vsip_cblockadmit_f(block,VSIP_TRUE);

     vsip_cmcopy_f_f(a,b);

     val = vsip_mcmaxmgsqval_f(a,&index);
     printf("val = vsip_mcminmgsqval_f(a,index)\n matrix a = \n");VU_cmprintm_f("8.6",a);
     printf("val = %f\n",val); 
     printf("index = (%ld, %ld)\n",vsip_colindex(index),vsip_rowindex(index));
     if(fabs(ans - val) > .0001)
        printf("value error\n");
     else
        printf("value correct\n");
     if((vsip_colindex(index) != vsip_colindex(ind_ans)) ||
        (vsip_rowindex(index) != vsip_rowindex(ind_ans)))
        printf("index error\n");
     else
        printf("index correct\n");

     printf("case for non-compact matrix with negative strides\n");
     val = vsip_mcmaxmgsqval_f(b,&index);
     printf("val = vsip_mcmaxmgsqval_f(b,index)\n matrix b = \n");VU_cmprintm_f("8.6",b);
     printf("val = %f\n",val); 
     printf("index = (%ld, %ld)\n",vsip_colindex(index),vsip_rowindex(index));
     if(fabs(ans - val) > .0001)
        printf("value error\n");
     else
        printf("value correct\n");
     if((vsip_colindex(index) != vsip_colindex(ind_ans)) ||
        (vsip_rowindex(index) != vsip_rowindex(ind_ans)))
        printf("index error\n");
     else
        printf("index correct\n");

     vsip_cmalldestroy_f(a);
     vsip_cmalldestroy_f(b);
   }
   return;
}
