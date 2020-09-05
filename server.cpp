#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

bool comp(char* c1, char* c2)
{
    return c1[0]==c2[0] && c1[1]==c2[1];
}

int main()
{
	int listenfd;
	if( (listenfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// servaddr.sin_addr.s_addr = htonl(INADDR_ADNY);
	servaddr.sin_addr.s_addr = inet_addr("172.17.0.17");
	servaddr.sin_port = htons(39711);
	
    cout << "Waiting..." << endl;
    
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))!=0)
	{
		perror("bind");
		close(listenfd);
		return -1;
	}
	
	if(listen(listenfd, 5)!=0)
	{
		perror("listen");
		close(listenfd);
		return -1;
	}
	
	int clientfd;
	int socklen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t*)&socklen);
	//clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t*)&socklen);
	printf("客户端(%s)已连接。\n", inet_ntoa(clientaddr.sin_addr));
	
    
    
    
    ifstream ifile;
    ifile.open("server.txt", ios::in);
    if(!ifile)
    {
        cerr << "Open file fail." << endl;
        exit(1);
    }
    char buffer[1024];
    char* subBuff;
    char idx[10];
    char cnt[10];
    
    memset(&buffer, 0, sizeof(buffer));
    ifile >> buffer;
    strncpy(idx, buffer, 2);
    subBuff = buffer + 3;
    
    int tmpCnt = 1;
    while(1)
    {
        int iret;
        
        if((iret=recv(clientfd, cnt, sizeof(cnt), 0))<=0)
        {
            printf("iret=%d", cnt);
            break;
        }
        //if(cnt==idx)
        if(comp(cnt, idx))
        {
            cout << "温 岚：" << subBuff << endl;
            memset(&buffer, 0, sizeof(buffer));
            ifile >> buffer;
            strncpy(idx, buffer, 2);
            subBuff = buffer + 3;
        }
        
        tmpCnt++;
        if(tmpCnt>30) break;
        
        
    }
	
	close(listenfd);
	close(clientfd);
	
    cout << endl;
    return 0;
}
