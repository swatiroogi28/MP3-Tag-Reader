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
        if(validate_input(argc,argv,&viewInfo)==e_success)
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