#ifndef CLIENT_NET_H
#define CLIENT_NET_H
/*
* 文件名：client_net.h
* 创建人：吴季孔
* 创建时间：2020-9-11
* 描述：客户端网络通信底层实现
*/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>


class client_base{
    public:
        //启动运行一个socket
        static int run_sock(const char *IP, int port, sockaddr_in &addr, sa_family_t AF = AF_INET);
        static int close_sock(int sock);
        //发送一个字符串
        static int send_str(int sock, const char *msg);
        //接收字符串
        static int receive_str(int sock, std::string &msg);
        //发送文件
        //成功返回0 文件不存在返回1
        static int send_file(int sock, const char *file_path);
        //接收文件
        static int receive_file(int sock, const char *path, const char *file_rename = NULL);

        static std::string get_filename(const char *file_path);

//        static std::string str_usrname;
//   inline     static void name_write(const std::string name);
//    inline     static std::string name_read();

    private:
        static const int send_buf_size = 1024;
        static const int recv_buf_size = 1024;

        static void exit_char(const char*msg);
};

//std::string client_base::str_usrname="";
//void client_base::name_write(const std::string name)
//{
//    str_usrname=name;
//}
//std::string client_base:: name_read()
//{
//   return str_usrname;
//}

inline int client_base::run_sock(const char *IP, int port, sockaddr_in &addr, sa_family_t AF){
    int sock = socket(AF, SOCK_STREAM, 0);
    if(sock<0){
        printf("socket err\n");
        exit(1);
    }
    ///
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_port = htons(port);
    ///
    if(connect(sock, (sockaddr *)&addr, sizeof(addr))<0){
        printf("connect err\n");
        exit(1);
    }
    return sock;
}

inline int client_base::close_sock(int sock){
    send_str(sock, "close");
    ::close(sock);
}

inline int client_base::send_str(int sock, const char *msg){
    //计算发送大小 发送块数量 余量大小
    int send_size = strlen(msg)+1, block_num = send_size/(send_buf_size), r_send_size = send_size%send_buf_size;
    char send_buf[send_buf_size];//发送缓冲区
    char recv_buf[recv_buf_size];//接收缓冲区
    //循环发送块
    for(int i=0;i<block_num;i++){
        memcpy(send_buf, msg+i*send_buf_size, sizeof(char)*send_buf_size);
        send(sock, send_buf, send_buf_size, 0);
        //对方确认接收后返回 "*#*#received#*#*"
        recv(sock, recv_buf, recv_buf_size, 0);
        if(strcmp(recv_buf, "*#*#received#*#*")!=0) exit_char("recv err 1\n");
    }
    //发送不足一块的余量
    if(r_send_size>0){
        memcpy(send_buf, msg+block_num*send_buf_size, sizeof(char)*r_send_size);
        send(sock, send_buf, r_send_size, 0);
        recv(sock, recv_buf, recv_buf_size, 0);
        if(strcmp(recv_buf, "*#*#received#*#*")!=0) exit_char("recv err 2\n");
    }
    //发送结束标志
    send(sock, "*#*#sendend#*#*", 16, 0);
    //printf("*#*#sendend#*#*\n");
    recv(sock, recv_buf, 9, 0);
    //printf("received \"%s\".\n", recv_buf);
    return send_size;//返回发送字节数
}

inline int client_base::receive_str(int sock, std::string &msg){
    msg.clear();
    char recv_buf[recv_buf_size+1];
    while (1){
        recv(sock, recv_buf, recv_buf_size, 0);
        recv_buf[recv_buf_size] = 0;
        if(strcmp(recv_buf, "*#*#sendend#*#*")==0){
            break;
        }
        send(sock, "*#*#received#*#*", 17, 0);
        msg+=recv_buf;
    }
    send(sock, "received", 9, 0);
    return msg.length();
}

inline int client_base::send_file(int sock, const char *file_path){
    struct stat file_ifo;
    if(stat(file_path, &file_ifo)<0) return 1;
    int send_size = file_ifo.st_size, block_num = send_size/(send_buf_size), r_send_size = send_size%send_buf_size;
    std::ifstream file(file_path, std::ios::in|std::ios::binary);
    if(!file.is_open()) return 1;
    char send_buf[send_buf_size];
    char recv_buf[recv_buf_size];
    std::string send_ifo = "*#*#sendfile#*#*/"+get_filename(file_path)+"/"+std::to_string(block_num)+"/"+std::to_string(r_send_size);
    send_str(sock, send_ifo.c_str());
    for(int i = 0; i<block_num; i++){
        file.read(send_buf, send_buf_size);
        send(sock, send_buf, send_buf_size, 0);
        recv(sock, recv_buf, recv_buf_size, 0);
        if(strcmp(recv_buf,"*#*#received#*#*")!=0) exit_char("bag err!\n");
    }
    if(r_send_size>0){
        file.read(send_buf, r_send_size);
        send(sock, send_buf, r_send_size, 0);
        recv(sock, recv_buf, recv_buf_size, 0);
        if(strcmp(recv_buf,"*#*#received#*#*")!=0) exit_char("bag err!\n");
    }
    send(sock, "*#*#sendend#*#*", 16, 0);
    //printf("*#*#sendend#*#*\n");
    recv(sock, recv_buf, 9, 0);
    //printf("received \"%s\".\n", recv_buf);
    file.close();
    return 0;
}

inline int client_base::receive_file(int sock, const char *path,const char *file_rename){
    if(access(path, F_OK)<0){
        mkdir(path, 0777);
    }
    char send_buf[send_buf_size];
    char recv_buf[recv_buf_size];
    int block_num = 0, r_recv_size = 0;
    std::string recv_bag ,file_name;
    receive_str(sock, recv_bag);
    ///提取文件名 块数量 余量
    for(int i = 16, len = recv_bag.length(), x = 0;i<len; i++){
        if(recv_bag[i]=='/') x++;
        else if(x==1){
            file_name+=recv_bag[i];
        }else if(x==2){
            block_num *= 10;
            block_num += recv_bag[i]-'0';
        }else{
            r_recv_size *= 10;
            r_recv_size += recv_bag[i]-'0';
        }
    }
    if(file_rename){
        file_name = file_rename;
    }
    std::ofstream file(std::string(path)+"/"+file_name, std::ios::out|std::ios::binary);
    if(!file.is_open()) exit_char("open err!\n");
    for(int i = 0; i<block_num; i++){
        recv(sock, recv_buf, recv_buf_size, 0);
        file.write(recv_buf, recv_buf_size);
        send(sock, "*#*#received#*#*", 17, 0);
    }
    if(r_recv_size>0){
        recv(sock, recv_buf, r_recv_size, 0);
        file.write(recv_buf, r_recv_size);
        send(sock, "*#*#received#*#*", 17, 0);
    }
    recv(sock, recv_buf, r_recv_size, 0);
    if(strcmp(recv_buf,"*#*#sendend#*#*")!=0) exit_char("recv err!\n");
    send(sock, "received", 9, 0);
    file.close();
    return 0;
}

inline void client_base::exit_char(const char*msg){
    printf(msg);
    exit(1);
}

inline std::string client_base::get_filename(const char *file_path){
    int len = strlen(file_path);
    int i;
    std::string filename;
    for(i = len-1; i>=0; i--){
        if(file_path[i]=='/') {
            break;
        }
    }
    filename += file_path+i+1;
    return filename;
}

#endif // CLIENT_NET_H
