/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ggengh.cpp
 *     Functions for creating and handling colour tables.
 * @par Purpose:
 *     Allows high performance fading for indexed colour images.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfgentab.h"

#include "bffile.h"
#include "bfpalette.h"
#include "bfscreen.h"

unsigned char ghost_table_UNUSED[256*256];

TbResult LbGhostTableGenerate(ubyte *pal, short intens, const char *fname)
{
    if (LbFileLoadAt(fname, ghost_table) == Lb_FAIL)
    {
        int i, k;
        ubyte *colr_i;
        ubyte *colr_k;
        ubyte *tbl_o;

        tbl_o = ghost_table;
        colr_i = pal;
        for (i = 0; i < 256; i++)
        {
            int ri, gi, bi;
            ri = colr_i[0];
            gi = colr_i[1];
            bi = colr_i[2];

            colr_k = pal;
            for (k = 0; k < 256; k++)
            {
                int rk, gk, bk;
                rk = intens * (colr_k[0] - ri) / 100;
                gk = intens * (colr_k[1] - gi) / 100;
                bk = intens * (colr_k[2] - bi) / 100;
                *tbl_o = LbPaletteFindColour(pal, rk + ri, gk + gi, bk + bi);
                colr_k += 3;
                tbl_o++;
            }
            colr_i += 3;
        }
    }
    lbDisplay.GlassMap = ghost_table;
    return Lb_SUCCESS;
}

TbResult LbGhostTableLoad(ubyte *pal, short intens, const char *fname)
{
    int i, k;
    TbResult ret;

    ret = LbFileLoadAt(fname, ghost_table);
    lbDisplay.GlassMap = ghost_table;

    for (i = 0; i < 256; i++)
    {
      for (k = 0; k < i; k++)
      {
          ushort a, b;
          a = (i << 8) + k;
          b = (k << 8) + i;
          ghost_table[a] = ghost_table[b];
      }
    }
    return ret;
}

/******************************************************************************/
