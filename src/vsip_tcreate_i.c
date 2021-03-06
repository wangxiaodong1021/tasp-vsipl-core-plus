/* Created RJudd September 18, 2000 */
/* SPAWARSYSCEN D857                */
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
/* $Id: vsip_tcreate_i.c,v 2.1 2004/09/21 02:07:25 judd Exp $ */

#define VI_TVIEW_I_
#include"VI_support_priv_i.h"
#include"VI_blockcreate_i.h"
#include"VI_blockdestroy_i.h"

vsip_tview_i* vsip_tcreate_i(
     vsip_length zlength,
     vsip_length ylength,
     vsip_length xlength,
     vsip_tmajor major,
     vsip_memory_hint hint)
{
     vsip_block_i *block;
     vsip_tview_i *tview;
     block = VI_blockcreate_i(zlength * ylength * xlength,hint);
     if(block == NULL) return (vsip_tview_i*) NULL;
     tview = VI_tview_i();
     if(tview == NULL){
          VI_blockdestroy_i(block);
          return tview;
      }
     tview->block = block;
     tview->offset = (vsip_offset)0;
     tview->x_length = xlength;
     tview->y_length = ylength;
     tview->z_length = zlength;
     if(major == VSIP_TRAILING){
        tview->z_stride = xlength * ylength;
        tview->y_stride = xlength;
        tview->x_stride = 1;
     } else { /* VSIP_LEADING */
        tview->x_stride = zlength * ylength;
        tview->y_stride = zlength;
        tview->z_stride = 1;
     }
     tview->markings = vsip_valid_structure_object;
     return tview;        
}
