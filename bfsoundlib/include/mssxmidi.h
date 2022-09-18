/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxmidi.h
 *     Header file for mssxmidi.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AIL2OAL_MSSXMIDI_H_
#define AIL2OAL_MSSXMIDI_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop);
void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSXMIDI_H_
