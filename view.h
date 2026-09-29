#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include "types.h"
 // Contains user defined types

typedef struct _ViewInfo
{
    char *src_mp3_fname;
    FILE *fptr_mp3;



}ViewInfo;


/* Check operation type */
OperationType check_operation_type(char *argv);

Status validate_input(int argc,char *agrv[],ViewInfo *viewInfo);

Status do_view(ViewInfo *viewInfo);

Status open_files_view(ViewInfo *viewInfo);

Status read_tag(ViewInfo *viewInfo);
#endif
