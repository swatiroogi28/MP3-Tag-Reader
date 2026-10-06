#include<stdio.h>
#include<string.h>
#include"types.h"
#include"view.h"


int main(int argc,char*argv[])
{
    ViewInfo viewInfo;
    if(argc<2)
    {
        printf("USAGE:\n");
        printf("To view please pass like : ./a.out -v mp3filename\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
        return 1;
    }
    if(check_operation_type(argv[1])==e_view)
    {
        if(validate_input_view(argc,argv,&viewInfo)==e_success)
        {
            
            if(do_view(&viewInfo)==e_success)
            {
                printf("Success\n");
            }
            else
            {
                printf("failure\n");
            }
        }
    }
    else if(check_operation_type(argv[1])==e_edit)
    {
        if(validate_input_edit(argc,argv,&viewInfo)==e_success)
        {
            if(do_edit(&viewInfo)==e_success)
            {
                printf("Edited successfully\n");
            }
            else
            {
                printf("Edit unsuccessfull\n");
            }
        }
    }
    else if(check_operation_type(argv[1])==e_help)
    {
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");
        printf("\t2.1. -t -> to edit song title\n");
        printf("\t2.2. -a -> to edit artist name\n");
        printf("\t2.3. -A -> to edit album name\n");
        printf("\t2.4. -y -> to edit year\n");
        printf("\t2.5. -m -> to edit content\n");
        printf("\t2.5. -c -> to edit comment\n");

    }
    else
    {
        printf("USAGE:\n");
        printf("To view please pass like : ./a.out -v mp3filename\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
    }
}

OperationType check_operation_type(char *argv)
{
    if(strcmp(argv,"-e")==0)
    {
        return e_edit;
    }
    else if(strcmp(argv,"-v")==0)
    {
        return e_view;
    }
    else if(strcmp(argv,"--help")==0)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}