#ifndef IDENTIFY_H
#define IDENTIFY_H
#pragma once
#include "client_net.h"

class Identify : client_base{
public:
    int to_check(int client_sck,char *msg);
    int to_register(int client_sck,char *msg);
private:

};

///msg为发送的用户名和密码，中间以“/”分隔
///函数返回值为0代表验证成功，返回值为1代表密码错误，返回值为-1代表用户名错误，返回值为2代表未收到反馈信息
inline int Identify::to_check(int client_sck,char *msg){
   std::string recv_buf;
   send_str(client_sck, "login");
   send_str(client_sck, msg);
   receive_str(client_sck,recv_buf);

   if(recv_buf=="right"){
       printf("0\n");
        return 0;
   } else if(recv_buf=="wrong username"){
       printf("-1\n");
        return -1;
   } else if(recv_buf=="wrong password"){
       printf("1\n");
        return 1;
   } else {
       printf("2\n");
        return 2;
   }
}
///msg为发送的用户名/昵称/密码
///函数返回值0为注册成功,返回值为1代表用户名存在，返回值为2代表未收到反馈信息
inline int Identify::to_register(int client_sck,char *msg)
{
   std::string recv_buf;
   send_str(client_sck, "register");
   send_str(client_sck, msg);
   receive_str(client_sck, recv_buf);
   if(recv_buf=="register success"){
        return 0;
   }
   else if(recv_buf=="register fail"){
        return 1;
   }else{
        return 2;
   }
}
#endif // IDENTIFY_H
