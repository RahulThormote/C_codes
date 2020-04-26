#include"headers.h"

void read_file();
void Modify_Entry();
void Database_Menu();
void Remove_Entry();
void Add_Entry();

unsigned char PageArray[20];
unsigned char **file='\0',read_flag=0,flag=0;;
unsigned int line_cnt=0,tempcnt=0,pageno,size,page_cnt=0,entry,addr;
FILE *fp;
unsigned char ip[4],ipaddr[16];
unsigned long long imei;
struct in_addr addrscfg;

void Edit_Configuration()
{	
	unsigned int lin_fil=0,j=0,i=0;
	unsigned char *str,choice;//ipaddr[16];

	if(!read_flag)
 		read_file();
	
	Database_Menu();

	if((fp=fopen("M2MDatabase.txt","a+"))==NULL)
	{
		printf("File Doesn't Exit:\n ");
		return;
	}

	fseek(fp,0,2);
	size=ftell(fp)+1;
	rewind(fp);
// 	printf("\nsize:%d\n",size);
	str=calloc(1,size);
	
	fread(str,size-1,1,fp);
 //	puts(str);

	PageArray[pageno++]=i;
		
	while(str[i]!='\0')
	{
		if(str[i++]=='\n')
		{
			
			PageArray[pageno]=i;
// 			printf("pageArray[%d]:%d\n",pageno,PageArray[pageno]);
			pageno++;
		}
	}
	free(str);
 //	printf("linecnt:%d\n",line_cnt);

	while(tempcnt!=line_cnt)
	{
 	//	printf("temp:%d line_cnt:%d\n",tempcnt,line_cnt);
		sscanf(file[tempcnt++],"%llu %s\n",&imei,ipaddr);
		page_cnt++;
		j++;
		if((page_cnt%15)==1)
		{ 
			Database_Menu();
		}
		printf("%4d  %18llu	%10s\n",tempcnt,imei,ipaddr);
		if(j>=15)
		{
			if(page_cnt==15)
		        {
				printf("\n\r ---------------------------------------------------------");
        			printf("\n\r Press N -> Next Page     P -> Previous Menu     ");
				printf("\n\r ---------------------------------------------------------\n\r");
// 				printf("\r Enter your choice:");
// 				scanf(" %c",&choice);
				j=0;
				while(1)
				{
					printf("n\r Enter your choice:");
	 				scanf(" %c",&choice);
					if(choice=='P')
					{
						pageno=0;
						tempcnt=0;
						page_cnt=0;
						system("clear");
						return;
					}
					else if(choice == 'N')
					{
						break;
					}
					else
						printf("\n\r Invalid choice:\n");

				}
// 				while(choice!='N')
// 				{
// 					printf(" Wrong option please enter again:\n");
// 					printf(" Enter your choice:");
// 					scanf(" %c",&choice);
// 					
// 				}
				printf("\n");
			}
			else if(page_cnt>15)
			{
			     if(page_cnt%15==0)
			     {
				  printf("\n\r -----------------------------------------------------------");
        			  printf("\n\r Press N -> Next Page, P -> Prev Page");
			          printf("\n\r-----------------------------------------------------------\n");
				  while(1)
				  {
					printf("\n\r Enter your choice:");
				  	scanf(" %c",&choice);
				  	if(choice=='P')
				  	{
PREV:
			//              	printf("%d %d\n",page_cnt,tempcnt);
						if(flag==1)
						{
							page_cnt=page_cnt-(page_cnt%15)-15;
							tempcnt=tempcnt-(tempcnt%15)-15;
							flag=0;	
						}
						else
						{
							page_cnt=page_cnt-(page_cnt%15)-30;
							tempcnt=tempcnt-(tempcnt%15)-30;
							
						}

						fseek(fp,-(PageArray[page_cnt]),SEEK_CUR);
						break;
				  	}
				  	else if(choice=='N')
						break;
				  	else
				  	{
						printf("\n\r Invalid choice:\n");
				 	}
				  }
				  printf("\n");
				
			     }
			}
		}	
	}
 	printf("\n\r ---------------------------------------------------------");
 	printf("\n\r %d is Last Entry:\n",tempcnt);
	printf("\n\r ---------------------------------------------------------");
        printf("\n\r Press P -> Prev Page, N -> Next Menu ");
	printf("\n\r ---------------------------------------------------------\n");
	while(1)
	{
 		printf("\n\r Enter your choice : ");
		scanf(" %c",&choice);
		fflush(stdin);
		if(choice=='P')
		{
			flag=1;
			goto PREV;
		}
		else if(choice=='N')
			break;
		else
			printf(" Invalid choice:\n");
		
	}
	tempcnt=0;
	page_cnt=0;
	line_cnt=0;
	read_flag=0;
	free(file);
	fclose(fp);

	system("clear");

	while(1)
	{
		printf("\n\n\r --------------------------------------------------------\n\r");
		printf("\n\r        M2M Gateway Configuration \n\r");
	        printf("\n\r --------------------------------------------------------\n\r");
		printf("\n\r  1. Add entry \n\r");
	        printf("\n\r  2. Modify entry \n\r");
		printf("\n\r  3. Delete entry\n\r");
	        printf("\n\r  4. Exit \n\r");
		printf("\n\r --------------------------------------------------------");
	        printf("\n\r Enter your choice : ");
		scanf("%d",&choice);
	//	fflush(stdin);
		getchar();
		switch(choice)
		{
			case 1:
				Add_Entry();
				read_flag=0;
				break;
			case 2:
				Modify_Entry();
				read_flag=0;
				break;
			case 3:
				Remove_Entry();
				read_flag=0;
				break;
			case 4:
				printf("\n Back to Main Menu:\n");
				system("clear");
				return;
				
			default:
				printf(" Invalid choice:\n");

		}
	}
}

void read_file(void)
{
	unsigned char*line;
	unsigned int lin_fil=0;
	line_cnt=0;
	fp=fopen("M2MDatabase.txt","r");
	if(!fp)
	{
		printf("file not present\n");
		return;	
	}
	
	line=(unsigned char *)calloc(1,50);

	rewind(fp);
	while(fgets(line,50,fp))	
	{
		line_cnt++;	///counting lines
	}
	free(line);
	
//	printf("linecnt1:%d\n",line_cnt);
	file=(unsigned char **)calloc(line_cnt,sizeof(char *));	///allocate no. of char pointers for line
	
	fseek(fp,0,0);       //R->Back to Start
	while(lin_fil < line_cnt)
	{
		file[lin_fil]=(unsigned char *)calloc(1,50);
		fgets(file[lin_fil++],50,fp);
	}
	fclose(fp);	
	read_flag=1;

}

void Modify_Entry()
{
	unsigned int temp=0;
	unsigned char *en_ptr='\0';

	printf("\n Enter Entry To Modify:-");
	scanf("%d",&entry);
	
	if(!read_flag)		
		read_file();

	if(entry>line_cnt)
	{
		printf("\n Entry not Present,cannot modify:\n\n");
		return;
	}

	printf("\n Enter New IMEI:");
 	scanf("%llu",&imei);
// 	printf("\n Enter new IP:-");
// 	scanf("%s",ipaddr);
// 
// 	en_ptr=file[entry-1];
// 	free(en_ptr);
// 	en_ptr=(unsigned char *)calloc(1,50);
// 	sprintf(en_ptr,"%llu %s\n",imei,ipaddr);
// 	entry=0;
// 	
// 	if(entry)	return;
// 	
// 	unlink("M2MDatabase.txt");
// 	fp=fopen("M2MDatabase.txt","w");
// 	while(temp<line_cnt)
// 	{
// 		fputs(file[temp++],fp);
// 	}
// 	fclose(fp);
// 	printf("\n Entry Modified Successfully\n");

	printf("\n\r Enter Local IP Address : ");
	scanf("%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
	addr = ((unsigned int)((ip[3]) & 0xff) << 24) | \
	       ((unsigned int)((ip[2]) & 0xff) << 16) | \
               ((unsigned int)((ip[1]) & 0xff) << 8)  | \
               (unsigned int)((ip[0]) & 0xff);
	addrscfg.s_addr = addr;

	en_ptr=file[entry-1];
	free(en_ptr);
	en_ptr=(unsigned char *)calloc(1,50);
	sprintf(en_ptr,"%llu %s\n",imei,inet_ntoa(addrscfg));
	entry=0;
	
	if(entry)	return;
	
	unlink("M2MDatabase.txt");
	fp=fopen("M2MDatabase.txt","w");
	while(temp<line_cnt)
	{
		fputs(file[temp++],fp);
	}
	fclose(fp);
	printf("\n Entry Modified Successfully\n");
}


void Remove_Entry()
{
	unsigned int temp=0;
	unsigned char *en_ptr='\0';	

	printf("\n Enter the entry to delete:-");
	scanf("%d",&entry);

	if(!read_flag)		
		read_file();

	if(entry>line_cnt)
	{
		printf("\n Entry not Present,cannot Remove:\n\n");
		return;
	}

	en_ptr=file[entry-1];
	unlink("M2MDatabase.txt");
	fp=fopen("M2MDatabase.txt","w");
	//printf("entry:%d line_cnt:%d\n",entry,line_cnt);

	while(temp<line_cnt)
	{
		if((temp+1)!=entry)   //4
		{
			fputs(file[temp],fp);    
			temp++;
		}
		else
		{
			if(entry==line_cnt)
			{
				break;
			}
			fputs(file[++temp],fp);
			temp++;
		}
	}
	free(en_ptr);
	read_flag=0;
	fclose(fp);
	printf("\n Entry %d Deleted Successfully\n",entry);
}



void Add_Entry()
{
	fp=fopen("M2MDatabase.txt","a+");
	if(fp=='\0')
	{
		printf("File cannot Open:\n");
		return;
	}
// 	fp=fopen("M2MDatabase.txt","a+");
	//fseek(fp,0,2);
 	//fputc('\n',fp);
	printf("\n Enter Imei No:");
	scanf("%llu",&imei);
	printf("\n Enter Local IP Address :");
	scanf("%s",ipaddr);
	fprintf(fp,"%llu %s\n",imei,ipaddr);
	fclose(fp);
	printf("\n Entry appended SuccessFully\n");

	
// 	printf("\n Enter Imei No:");
// 	scanf("%llu",&imei);
// 	printf("\n\r Enter Local IP Address : ");
// 	scanf("%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
// 	addr = ((unsigned int)((ip[3]) & 0xff) << 24) | \
// 	       ((unsigned int)((ip[2]) & 0xff) << 16) | \
//                ((unsigned int)((ip[1]) & 0xff) << 8)  | \
//                (unsigned int)((ip[0]) & 0xff);
// 
// 	printf("addr:%x\n",addr);
//  	addrscfg.s_addr = addr;
// 
// 	fprintf(fp,"%llu %s\n",imei,inet_ntoa(addrscfg));
//  	fclose(fp);
//  	printf("\n Entry appended SuccessFully\n");
}

void Add_Given_Pos()
{
	unsigned int temp=0,i=0;
	unsigned char **buf,buffer[50];
	unsigned char *en_ptr='\0';
	fp=fopen("M2MDatabase.txt","r+");
	if(!read_flag)		
		read_file();

	buf=(unsigned char **)calloc(1,50);
	for(i=0;i<line_cnt;i++)
	{
		buf[i]=calloc(1,50);
		strcpy(buf[i],file[i]);
	}

	printf("\n Enter position you wonna add:");
	scanf("%d",&entry);
	
	/*en_ptr=file[entry-1];
	ptr=buf[entry]*/;
	unlink("M2MDatabase.txt");
	fp=fopen("M2MDatabase.txt","a");
	while(temp<line_cnt)
	{
		
		if((temp+1)!=entry)   //5
		{
		
			fputs(file[temp],fp);
			temp++;
		}
		else
		{
			printf("\n Enter IMEI:");
			scanf("%llu",&imei);
			printf("\n\r Enter Local IP Address : ",imei);
			scanf("%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
			addr = ((unsigned int)((ip[3]) & 0xff) << 24) | \
	       			((unsigned int)((ip[2]) & 0xff) << 16) | \
               			((unsigned int)((ip[1]) & 0xff) << 8)  | \
               			(unsigned int)((ip[0]) & 0xff);
			addrscfg.s_addr = addr;
			sprintf(buffer,"%llu %s\n",imei,inet_ntoa(addrscfg));
			fputs(buffer,fp);
			break;
		}
	}
	while(temp<line_cnt)
	{
		fputs(buf[temp],fp);
		temp++;
	}
	fclose(fp);
	printf("\n Entry added at perticular position Successfully\n");
}




void Database_Menu()
{

	system("clear");
	printf("\f\n\r ---------------------------------------------------------\n\r");
	printf("\n\r        M2M Gateway Database        \n\r");
	printf("\n\r ---------------------------------------------------------");
        printf("\n\r S.No.    IMEI No.              Local IP   ");
	printf("\n\r ---------------------------------------------------------\n\r");
}


int main()
{
	unsigned int ch;
	while(1)
	{
// 		system("clear");
		printf("\n\n\r --------------------------------------------------------\n\r");
		printf("\n\r        M2M Gateway Configuration \n\r");
		printf("\n\r --------------------------------------------------------\n\r");
// 		printf("\n\r 1. Add Entries \n\r");
// 	        printf("\n\r 2. Modify Configuration\n\r");
// 		printf("\n\r 3. Print Pagewise\n\r");
// 		printf("\n\r 4. Remove Entry\n\r");
// 		printf("\n\r 5. Add At Given Position\n\r");
		printf("\n\r 1.Edit Configuration\n\r");
		printf("\n\r 2.Exit\n\r");
		printf("\n\r --------------------------------------------------------");
ENTER:
	        printf("\n\r Enter your choice :");
		scanf("%d",&ch);
		getchar();
// 		fflush(stdin);
		switch(ch)
		{
			
			case 1:Edit_Configuration();
				break;

//  			case 5:Add_Given_Pos();
//  				read_flag=0;
//  				break;
			case 2:printf(" Logout Successfully....Thanks\n");
				exit(0);
				break;	
			default:
				printf("\n Invalid choice:\n"); 	
				usleep(100000);
 				goto ENTER;
		}
	}
	return 0;
}
