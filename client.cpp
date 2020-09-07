
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

int str2Int(string a)
{
    int x, y;
    x = a[0] - '0';
    y = a[1] - '0';
    return x * 10 + y;
}

void charIncr(char *ch)
{
    if(ch[1]=='9')
    {
        ch[1] = '0';
        ch[0] = ch[0] + 1;
    }
    else
    {
        ch[1] = ch[1] + 1;
    }
    return;
}

bool comp(char* c1, char* c2)
{
    return c1[0]==c2[0] && c1[1]==c2[1];
}

int main()
{
    int sockfd;
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        perror("socket");
        return -1;
    }

    struct hostent* h;
    if((h=gethostbyname("111.229.178.76"))==0)
    {
        printf("gethostbyname failed.\n");
        close(sockfd);
        return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(39711);
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
    
    
    while(1)
    {
        string tmpStr;
        getline(cin, tmpStr);
        if(tmpStr=="Go!")
        {
            break;
        }
    }
    
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!=0)
    {
        perror("connect");
        close(sockfd);
        return -1;
    }
    
    ifstream ifile;
    ifile.open("client.txt", ios::in);
    if(!ifile)
    {
        cerr << "Open file fail." << endl;
        exit(1);
    }
    
    sleep(1);
    char idx[10];
    char cnt[10] = "01";
    char buffer[1024];
    char* subBuff;
    
    memset(&buffer, 0, sizeof(buffer));
    ifile >> buffer;
    strncpy(idx, buffer, 2);
    subBuff = buffer + 3;
    int sec[] = {16, 11, 10, 11, 1, 2, 6, 3, 3, 2, 3, 9, 2, 2, 3, 3, 3, 3, 6, 2};
    int secIdx = 0;
    for(int i=1; i<=21; ++i)
    {
        int iret;
        
        if((iret=send(sockfd, cnt, sizeof(cnt), 0))<=0)
        {
            printf("iret=%d\n", iret);
            break;
        }
        
        //if(cnt==idx)
        if(comp(cnt, idx))
        {
            cout << "周杰伦：" << subBuff << endl;
            memset(&buffer, 0, sizeof(buffer));
            ifile >> buffer;
            strncpy(idx, buffer, 2);
            subBuff = buffer + 3;
        }
        
        sleep(sec[secIdx++]);
        charIncr(cnt);
    }
    
    ifile.close();
    close(sockfd);
	
}
