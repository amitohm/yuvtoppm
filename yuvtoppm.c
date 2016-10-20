#include "yuvtoppm.h"


////////////
//main
////////////
INT32 main(INT32 argc, INT8 *argv[])
{
	FILE *fp;
	
	INT8 *in_fname,*out_fname,filename[512];
	UINT32 rows,cols,depth,i,j;
	INT32 temp;
	UINT8 *Yptr,*Cbptr,*Crptr;
	
	////////////////
	// image read 
	////////////////
	
	in_fname = out_fname = NULL;
	cols = rows = 0;
	
	i = 1;
	while(argv[i]!=NULL)
	{
		if(strcmp(argv[i],"-w")==0)
		{
			i++;
			cols = atoi(argv[i]);
		}
		else if(strcmp(argv[i],"-h")==0)
		{
			i++;
			rows = atoi(argv[i]);
		}
		else if(strcmp(argv[i],"-o")==0)
		{
			i++;
			out_fname = argv[i];
		}
		else
		{
			in_fname = argv[i];
		}
		
		i++;
	}
	
	if((in_fname==NULL) || (cols==0) || (rows==0))
	{
		print_usage();
	}
	
	depth = 255;
	
	if ((fp = fopen(in_fname,"rb"))==NULL)
	{
		printf("Error accessing %s.\n",in_fname);
		exit(-1);
	}
	
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	temp = ftell(fp);
	
	fseek(fp, 0, SEEK_SET);
	
	if(temp<(3*cols*rows))
	{
		printf("Unsupported YUV format.\n");
		printf("Input file should be YUV444 format.\n");
		print_usage();
	}
	
	j = 0;
	for(i=0; in_fname[i] != '\0';i++)
	{
		if(in_fname[i] == '/')
			j = i + 1;
	}
	
	i = 0;
	while(in_fname[j] != '.')
	{
		in_fname[i] = in_fname[j];
		i++;
		j++;
	}
	in_fname[i] = '\0';
	
	/////////////
	// mallocs //
	/////////////
	Yptr = (UINT8 *)malloc(cols*rows*sizeof(UINT8));
	if(Yptr==NULL)
	{
		printf("Error allocating memory for Yptr.\n");
		exit(-1);
	}
	Cbptr = (UINT8 *)malloc(cols*rows*sizeof(UINT8));
	if(Cbptr==NULL)
	{
		printf("Error allocating memory for Cbptr.\n");
		exit(-1);
	}
	Crptr = (UINT8 *)malloc(cols*rows*sizeof(UINT8));
	if(Crptr==NULL)
	{
		printf("Error allocating memory for Crptr.\n");
		exit(-1);
	}
	
	fread(Yptr,sizeof(UINT8),(cols*rows),fp);
	fread(Cbptr,sizeof(UINT8),(cols*rows),fp);
	fread(Crptr,sizeof(UINT8),(cols*rows),fp);
	fclose(fp);
	
	if(out_fname==NULL)
	{
		sprintf(filename,"%s.ppm", in_fname);
		out_fname = filename;
	}
	fp = fopen(out_fname,"wb");
	if(fp==NULL)
	{
		printf("Error accessing %s.\n",out_fname);
		exit(-1);
	}
	fprintf (fp,"P6\n%d %d\n%d\n",cols,rows,depth);
	
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			temp = (INT32)((1.164*(Yptr[(i*cols)+j]-16)) + (1.596*(Crptr[(i*cols)+j]-128)));
			temp = MAX(temp, 0);
			temp = MIN(temp, 255);
			fputc(temp,fp);
			
			temp = (INT32)((1.164*(Yptr[(i*cols)+j]-16)) - (0.391*(Cbptr[(i*cols)+j]-128)) - (0.813*(Crptr[(i*cols)+j]-128)));
			temp = MAX(temp, 0);
			temp = MIN(temp, 255);
			fputc(temp,fp);
			
			temp = (INT32)((1.164*(Yptr[(i*cols)+j]-16)) + (2.018*(Cbptr[(i*cols)+j]-128)));
			temp = MAX(temp, 0);
			temp = MIN(temp, 255);
			fputc(temp,fp);
		}
	}
	
	fclose(fp);
	
	free(Yptr);
	free(Cbptr);
	free(Crptr);
	
	return 0;
}

void print_usage()
{
	printf("yuvtoppm --help\t:\tDisplay this message.\n");
	printf("-w\t\twidth of YUV image\n");
	printf("-h\t\theight of YUV image\n");
	printf("-o\t\toutput filename\n");
	printf("\nExample: yuvtoppm -w <width> -h <height> <inputfile> -o <outputfile>\n");
	exit(-1);
}
