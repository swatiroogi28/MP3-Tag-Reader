#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"
#include "types.h"

Status validate_input_edit(int argc,char *argv[],ViewInfo *viewInfo)
{
    if(argc!=5)
    {
        printf("USAGE:\n");
        printf("To view please pass like : ./a.out -v mp3filename\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
        return e_failure;
    }
    if(strcmp(argv[2],"-t")!=0 && strcmp(argv[2],"-a")!=0 && strcmp(argv[2],"-A")!=0 && strcmp(argv[2],"-y")!=0 && strcmp(argv[2],"-m")!=0 && strcmp(argv[2],"-c")!=0)
    {
        printf("Invalid Edit option\n");
        return e_failure;
    }
    viewInfo->option=argv[2];
    viewInfo->newdata=argv[3];
    int len=strlen(argv[4]);
    if(len<4||argv[4][len-4]!='.'|| argv[4][len-3]!='m'|| argv[4][len-2]!='p'|| argv[4][len-1]!='3')
    {
        printf("Error: source file should be .mp3\n");
        return e_failure;
    }
    viewInfo->src_mp3_fname=argv[4];
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

Status open_files_edit(ViewInfo *viewInfo)
{
    viewInfo->fptr_mp3=fopen(viewInfo->src_mp3_fname,"r+");
    if (viewInfo->fptr_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", viewInfo->src_mp3_fname);

    	return e_failure;
    }
    return e_success;
}

Status do_edit(ViewInfo *viewInfo)
{
   
    char *options[]={"-t","-a","-A","-m","-y","-c"};
    char *tags[]={"TIT2","TPE1","TALB","TCON","TYER","COMM"};
    int i;
    for(i=0;i<6;i++)
    {
        if(strcmp(viewInfo->option,options[i])==0)
        {
            break;
        }
    }
    FILE *fptr_temp=fopen("temp.mp3","w");
    if(fptr_temp==NULL)
    {
        return e_failure;
    }
    rewind(viewInfo->fptr_mp3);
    char header[10];
    fread(header,1,10,viewInfo->fptr_mp3);
    fwrite(header,1,10,fptr_temp);
    char tag[5];
    char size[4];
    char flag[2];
    char null;
    int frame_size;

    fread(tag,1,4,viewInfo->fptr_mp3);
    tag[4]='\0';
    
    while(strcmp(tag,tags[i])!=0)
    {
        fwrite(tag,1,4,fptr_temp);
    
        fread(size,1,4,viewInfo->fptr_mp3);
        frame_size=((unsigned char)size[0]<<24)|((unsigned char)size[1]<<16)|((unsigned char)size[2]<<8)|(unsigned char)size[3];
        fwrite(size,1,4,fptr_temp);
        char data[frame_size-1];

        fread(flag,1,2,viewInfo->fptr_mp3);
        fwrite(flag,1,2,fptr_temp);

        fread(&null,1,1,viewInfo->fptr_mp3);
        fwrite(&null,1,1,fptr_temp);

        fread(data,1,frame_size-1,viewInfo->fptr_mp3);
        fwrite(data,1,frame_size-1,fptr_temp);

        fread(tag,1,4,viewInfo->fptr_mp3);
        tag[4]='\0';

    }
    fwrite(tag,1,4,fptr_temp);
    fread(size,1,4,viewInfo->fptr_mp3);

    frame_size = ((unsigned char)size[0] << 24) |
                 ((unsigned char)size[1] << 16) |
                 ((unsigned char)size[2] << 8) |
                 (unsigned char)size[3];
    
    int new_size=strlen(viewInfo->newdata)+1;
    char new_size_bytes[4];
    new_size_bytes[0]=(new_size>>24)&0xFF;
    new_size_bytes[1]=(new_size>>16)&0xFF;
    new_size_bytes[2]=(new_size>>8)&0xFF;
    new_size_bytes[3]=new_size&0xFF;
    fwrite(new_size_bytes,1,4,fptr_temp);

    fread(flag,1,2,viewInfo->fptr_mp3);
    fwrite(flag,1,2,fptr_temp);

    null='\0';
    fwrite(&null,1,1,fptr_temp);

    fwrite(viewInfo->newdata,1,strlen(viewInfo->newdata),fptr_temp);
    
    fread(&null,1,1,viewInfo->fptr_mp3);

    fseek(viewInfo->fptr_mp3,frame_size-1,SEEK_CUR);
   
    char buffer[1024];
    int bytes;

    while((bytes=fread(buffer,1,1024,viewInfo->fptr_mp3))>0)
    {
            fwrite(buffer,1,bytes,fptr_temp);
    }
    fclose(fptr_temp);

    remove(viewInfo->src_mp3_fname);

    rename("temp.mp3",viewInfo->src_mp3_fname);
    
    return e_success;

}