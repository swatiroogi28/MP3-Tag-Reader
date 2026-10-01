#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include "types.h"
 // Contains user defined types

typedef struct _ViewInfo
{
    char *src_mp3_fname;
    FILE *fptr_mp3;
    char *option;
    char *newdata;

}ViewInfo;


/* Check operation type */
OperationType check_operation_type(char *argv);

Status validate_input_view(int argc,char *agrv[],ViewInfo *viewInfo);

Status open_files_view(ViewInfo *viewInfo);

Status do_view(ViewInfo *viewInfo);

Status read_tag(ViewInfo *viewInfo);

Status validate_input_edit(int argc,char *agrv[],ViewInfo *viewInfo);

Status open_files_edit(ViewInfo *viewInfo);

Status do_edit(ViewInfo *viewInfo);

#endif
