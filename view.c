#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "view.h"
#include "types.h"

Status validate_input(int argc,char *argv[],ViewInfo *viewInfo)
{
    //check if the input is .mp3 file 
    //check signature is ID3 or not
    if(argc!=3)
    {
        printf("USAGE:\n");
        printf("To view please pass like : ./a.out -v mp3filename\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }
    int len=strlen(argv[2]);
    if(len<4||argv[2][len-4]!='.'|| argv[2][len-3]!='m'|| argv[2][len-2]!='p'|| argv[2][len-1]!='3')
    {
        printf("Error: source file should be .mp3\n");
        return e_failure;
    }
    viewInfo->src_mp3_fname=argv[2];
    if(open_files_view(viewInfo)==e_failure)
    {
        return e_failure;
    }
    char buff[4];
    fread(buff,1,3,viewInfo->fptr_mp3);
    buff[3]='\0';
    if(strcmp(buff,"ID3")!=0)
    {
        printf("Error: ID3 tag not found\n");
        return e_failure;
    }
    return e_success;

}

Status open_files_view(ViewInfo *viewInfo)
{
    viewInfo->fptr_mp3=fopen(viewInfo->src_mp3_fname,"r");
    if (viewInfo->fptr_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", viewInfo->src_mp3_fname);

    	return e_failure;
    }
    return e_success;
}
Status do_view(ViewInfo *viewInfo)
{
    if(fseek(viewInfo->fptr_mp3,10,SEEK_SET)!=0)
    {
        return e_failure;
    }
    if(read_tag(viewInfo)==e_failure)
    {
        return e_failure;
    }
    return e_success;

}
Status read_tag(ViewInfo *viewInfo)
{
    char tag[5];
    unsigned int size;
    unsigned char size_buff[4];
    char *data[6];
    char *tag_names[6]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
    char *display_names[6]={"Title","Artist","Album","year","Genre","Comment"};
    int i=0,j;
    while(i<6)
    {
        if(fread(tag,4,1,viewInfo->fptr_mp3)!=1)
        {
            return e_failure;
        }
        tag[4]='\0';
        if(fread(size_buff,4,1,viewInfo->fptr_mp3)!=1)
        {
            return e_failure;
        }
        size=((size_buff[0]<<24)|(size_buff[1]<<16)|(size_buff[2]<<8)|size_buff[3]);

        if(fseek(viewInfo->fptr_mp3,3,SEEK_CUR)!=0)
        {
            return e_failure;
        }
        for(j=0;j<6;j++)
        {
            if(strcmp(tag,tag_names[j])==0)
            {
                data[j]=malloc(size);
                if(data[j]==NULL)
                {
                    return e_failure;
                }
                if(fread(data[j],1,size-1,viewInfo->fptr_mp3)!=size-1)
                {
                    return e_failure;
                }
                data[j][size-1]='\0';
                i++;
                break;
            }
        }
        if(j==6)
        {
            if(fseek(viewInfo->fptr_mp3,size-1,SEEK_CUR)!=0)
            {
                return e_failure;
            }
        }
    }
    printf("<-----------------------------Start of view------------------------------->");
    printf("\n-------------------------------------------------------------------------\n");
    printf("SI.No\t|\tTAG\t|\tContent");
    printf("\n-------------------------------------------------------------------------\n");
    for(int k=0;k<6;k++)
    {
        printf("%d\t|\t%s\t|\t%s\n",k+1,display_names[k],data[k]);
    }
    printf("\n<-----------------------------End of view--------------------------------->\n");
    return e_success;
    
}