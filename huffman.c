/************************************************************
Copyright (C), 2017
FileName: huffman.c
Author: Zhao Pengfei 
Date: 12.17-12.18 
Version: 1.20 
Function List: 
1. --void ReadFile(int *key) //读取并解析文件，key指向储存文件的字符总种数
2. --void SelectMin(HuffmanTree HT,int len,int *s1,int *s2)//选取最小的两个s1,s2 
3. --HuffmanTree CreateHuffmanTree(int num)//返回申请的地址  num 叶子节点数 
4. --void CreatHuffmanCode(HuffmanTree HT,int n)//创建哈夫曼编码
5. --void CodeFile(HuffmanTree HT,int n)//对文件进行哈夫曼编码
6. --void TranslationFile(HuffmanTree HT,int n)//译码	
7. --void menu()//菜单
8. --int main(int argc, char *argv[]) /主函数
History: // 12.19 
<author> <time> <version > <desc>
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

typedef struct{
	char ch;//字符 
	char *code;//指向编码表 
	int weight;//权值 
	int parent,lchild,rchild;
}HTNode, *HuffmanTree;

#define MAX 1000 
//文件字符最大数 
#define CODEMAX 20
//编码最大位数 

struct{
	char ch;
	int num;
}b[60];//暂时储存文件解析的数据 

char txt[MAX];//储存文本 
int txtlen;//文件长度 
char codetxt[5000];//存放编码txt后的数组 

void ReadFile(int *key){
	int sum=0;
	int i,j,len;
	int flag=0;
	char filename[10];
	*key=0; 
	for(j=0;j<40;j++){//初始化 
		b[j].ch='\0';
		b[j].num=0;
	}
	printf("请输入文件名：");
	scanf("%s",filename);
	FILE *fp=fopen(filename,"r+");
	if(fp==NULL){
		printf("文件打开失败！\n");
		exit(0); 
	}

	fgets(txt,MAX,fp);
	printf("%s\n",txt);//输出文件 
	for(i=0;i<MAX;i++)
		if(txt[i]=='\0'){
			txtlen=i;//文件长度 
			break;
		}
	for(i=0;i<txtlen;++i,flag=0){//解析文件字符及对应的数量 
		for(j=0;j<(*key);++j){
			if(b[j].ch==txt[i]){
				b[j].num++;
				flag=1;
			}
		}
		if(flag==0){
			b[*key].ch=txt[i];
			b[*key].num+=1;
			(*key)++;
		}	 
	}
	printf("文件共有字符种数：%d\n文件总字符数：%d\n",*key,txtlen); 
	fclose(fp);
} 



void SelectMin(HuffmanTree HT,int len,int *s1,int *s2){ //选取最小的两个s1,s2 
	int i;
	int min=255;
	for(i=1;i<=len;++i)
		if(HT[i].parent==0)
			if(min>HT[i].weight){
				min=HT[i].weight;
				*s1=i;
			}				
	min=255;
	for(i=1;i<=len;++i){
		if(i==(*s1))
			continue;
		if(HT[i].parent==0)
			if(min>HT[i].weight){
				min=HT[i].weight;
				*s2=i;
			}				
	}
				
}

 HuffmanTree CreateHuffmanTree(int num){//返回申请的树地址  num 叶子节点数 
	int m,i;
	int n=num;//叶子节点数 
	int s1=0,s2=0;
	HuffmanTree HT;
	if(n<=1)
		return NULL;		
	m=2*n-1;
	HT=(HuffmanTree)malloc(sizeof(HTNode)*(m+1));

	for(i=1;i<=n;++i){
		HT[i].parent=0;
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].ch=b[i-1].ch;
		HT[i].weight=b[i-1].num;
		HT[i].code='\0'; 
	}
	for(i=n+1;i<=m;++i){
		HT[i].parent=0;
		HT[i].lchild=0;
		HT[i].rchild=0;
		HT[i].ch='\0';
	}
	
	for(i=n+1;i<=m;++i){
		SelectMin(HT,i-1,&s1,&s2);
		HT[s1].parent=i;HT[s2].parent=i;
		HT[i].lchild=s1;
		HT[i].rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
		HT[i].code=0;
	}
	return HT;
}

void CreatHuffmanCode(HuffmanTree HT,int n){
	int i,start,c,f;
	char *cd=(char *)malloc(sizeof(char)*CODEMAX); 
    cd[CODEMAX-1]='\0';   
    for(i=1; i<=n; i++)  
    {      
	    start=CODEMAX-1;  
	    c=i;  
	    f=HT[i].parent;  
	      
	        while(f!=0){  
	            --start;  
	            if(HT[f].lchild==c){  //left--0  right--1
	              
	                cd[start]='0';  
	            }  
	            else{  
	               
	                cd[start]='1';  
	            }  
	            c=f;  
	            f=HT[f].parent;  
	        }  	    
		HT[i].code=(char*)malloc(sizeof(char)*(CODEMAX-start));  		
	    strcpy(HT[i].code,&cd[start]);  //将得到的哈夫曼编码复制到对应的节点	    
    }       
    free(cd);   		
} 

void CodeFile(HuffmanTree HT,int n){
	int i,j;
	char filename[10];
	printf("请输入编码输出文件名（格式：*.txt）：");
	scanf("%s",filename);
	FILE *fp=fopen(filename,"w");
	if(fp==NULL){
		printf("文件建立失败！\n");
		exit(0); 
	}
	
	for(i=0;i<txtlen;++i){//依次读取文件数组的字符 ,找到对应的编码 
		for(j=1;j<=n;++j){
			if(txt[i]==HT[j].ch)
				fprintf(fp,HT[j].code);				
		}
	}
	fclose(fp);
	printf("编码写入文件成功！\n");
}

void TranslationFile(HuffmanTree HT,int n){
	int i,j,f;
	char filename[10];
	char hc;
	printf("请输入要译码文件名（格式：*.txt）：");
	scanf("%s",filename);
	FILE *fp=fopen(filename,"r");
	if(fp==NULL){
		printf("文件打开失败！\n");
		exit(0); 
	}
	while(!feof(fp)){
		hc=fgetc(fp);
		if(feof(fp))
			break;
		f=2*n-1;
		while(1){
			if(hc=='0') f=HT[f].lchild;	
			else f=HT[f].rchild;			
			if(HT[f].lchild==0&&HT[f].rchild==0){
				printf("%c",HT[f].ch);
				break;
			}
			else hc=fgetc(fp);
		}
	}
	
	fclose(fp);
}
void menu(){
	system("cls");
	printf("\n\n\n\n");
	printf("\t\t|-----------哈夫曼编译码器 -----------|\n");
	printf("\t\t|\n");
	printf("\t\t|\t1.读取文件并建立哈夫曼树\n");
	printf("\t\t|\t2.导出编码文件\n");
	printf("\t\t|\t3.译码并打印\n");
	printf("\t\t|\t4.显示编码表\n");
	printf("\t\t|\n");
	printf("\t\t|-------------------------------------|\n\n");
	printf("\t\t\t请按照正确逻辑顺序操作！\n");
	printf("\n");
	printf("\t\t\t请选择(0-4):");
	
} 
int main(int argc, char *argv[]) {
	int i;
	int n;//字符总类型数 
	int choice;
	HuffmanTree HT; 
	system("color f0\n"); //白底黑字
	menu();
	scanf("%d",&choice);	
	while(choice)
	{
		switch(choice)
		{
			case 1:
				ReadFile(&n); 
				HT=CreateHuffmanTree(n);//构建哈夫曼树 
				CreatHuffmanCode(HT,n);//创建哈弗曼编码 
				break; 
			case 2:
				CodeFile(HT,n);
				break;
			case 3:
				TranslationFile(HT,n);
				break;
			case 4:
				for(i=1;i<=n;++i)
					printf(">>>%2d\tch:%c\tweight:%d\tcode:%s\n",i,HT[i].ch,HT[i].weight,HT[i].code);
				break; 
			default:
				break;
		}
		getch();
		menu();
		scanf("%d",&choice);
	}

	return 0;
}
