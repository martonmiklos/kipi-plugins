/*============================================================
 * 
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Description : Hugin parser API
 *
 * Copyright (C) 2007 Daniel M German <dmgerman at uvic doooot ca>
 * Copyright (C) 2012 by Benjamin Girault <benjamin dot girault at gmail dot com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of 
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================ */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>

#include "tparser.h"
#include "tparserprivate.h"
#include "tparserdebug.h"
#include "panoParser.h"

int yyparse(void);

#define PANO_PARSER_VERSION "0.10"

extern pt_script script;


/*
 * This function is be called before the parser is used for the first time, and if it wants to be reused.
 * Remember, the parser is not REENTRANT
 */
int panoScriptParserReset(void)
{
    if (!panoScriptDataReset())
    {
        return FALSE;
    }

    /* There should not be anything allocated in script */
    panoScriptParserSetDefaults(&script);
    return TRUE;
}

void panoScriptParserSetDefaults(pt_script* ptr)
{
    /* This is where the defaults will be
     * At this point. Just clear the data structure. */
    bzero(ptr, sizeof(*ptr));

    /* but some parameters are meaningful when zero */
    script.pano.projection = -1;
}

int panoScriptParse(char* filename, pt_script* scriptOut)
{
    /* filename: input file */

    DEBUG_1("Starting to parse");

    if (!panoScriptParserReset())
    {
        fprintf(stderr, "This parser is not reentrant");
        return FALSE;
    }

    if (!panoScriptParserInit(filename))
    {
        return FALSE;
    }

    if (yyparse() == 0)
    {
        /* AT THIS POINT WE HAVE FINISHED PARSING */
        memcpy(scriptOut, &script, sizeof(pt_script));
        panoScriptParserClose();
        /* We do not call panoScriptParserDispose here because its allocated values
         * are still referenced in scriptOut */
        return TRUE;
    }
    else
    {
        DEBUG_1("Error in parsing\n");
    }

    panoScriptFree(&script);
    panoScriptParserClose();
    return FALSE;
}

void panoScriptFree(pt_script* ptr)
{
    int i;
    /* free all the data structures it uses */

#define FREE(a) if ((a) != NULL) free(a);

    /* ptr->pano */
    FREE(ptr->pano.outputFormat);

    /* ptr->inputImageSpec */
    for (i = 0; i < ptr->iInputImagesCount; i++)
    {
        FREE(ptr->inputImageSpec[i].name);
    }
    FREE(ptr->inputImageSpec);

    /* ptr->varsToOptimize */
    for (i = 0; i < ptr->iVarsToOptimizeCount; i++)
    {
        FREE(ptr->varsToOptimize[i].varName);
    }
    FREE(ptr->varsToOptimize);

    /* ptr->ctrlPointsSpec */
    FREE(ptr->ctrlPointsSpec);

    /* ptr->masks */
    for (i = 0; i < ptr->iMasksCount; i++)
    {
        FREE(ptr->masks[i]->points);
    }
    FREE(ptr->masks);

    /* Comments */
    for (i = 0; i < ptr->iPano_prevCommentsCount; i++) {
        free(ptr->pano_prevComments[i]);
    }
    FREE(ptr->pano_prevComments);

    for (i = 0; i < ptr->iInputImagesCount; i++) {
        int j;
        for (j = 0; j < ptr->iImage_prevCommentsCount[i]; j++) {
            free(ptr->image_prevComments[i][j]);
        }
        FREE(ptr->image_prevComments[i]);
    }
    free(ptr->iImage_prevCommentsCount);
    free(ptr->image_prevComments);

    for (i = 0; i < ptr->iOptimize_prevCommentsCount; i++) {
        free(ptr->optimize_prevComments[i]);
    }
    FREE(ptr->optimize_prevComments);

    for (i = 0; i < ptr->iVarsToOptimizeCount; i++) {
        int j;
        for (j = 0; j < ptr->iVarsToOptimize_prevCommentsCount[i]; j++) {
            free(ptr->varsToOptimize_prevComments[i][j]);
        }
        FREE(ptr->varsToOptimize_prevComments[i]);
    }
    free(ptr->iVarsToOptimize_prevCommentsCount);
    free(ptr->varsToOptimize_prevComments);

    for (i = 0; i < ptr->iCtrlPointsCount; i++) {
        int j;
        for (j = 0; j < ptr->iCtrlPoints_prevCommentsCount[i]; j++) {
            free(ptr->ctrlPoints_prevComments[i][j]);
        }
        FREE(ptr->ctrlPoints_prevComments[i]);
    }
    free(ptr->iCtrlPoints_prevCommentsCount);
    free(ptr->ctrlPoints_prevComments);

    for (i = 0; i < ptr->iMasksCount; i++) {
        int j;
        for (j = 0; j < ptr->iMasks_prevCommentsCount[i]; j++) {
            free(ptr->masks_prevComments[i][j]);
        }
        FREE(ptr->masks_prevComments[i]);
    }
    free(ptr->iMasks_prevCommentsCount);
    free(ptr->masks_prevComments);

    for (i = 0; i < ptr->iEndingCommentsCount; i++) {
        free(ptr->endingComments[i]);
    }
    FREE(ptr->endingComments);

#undef FREE

    /* clear the data structure */
    panoScriptParserSetDefaults(ptr);
}

