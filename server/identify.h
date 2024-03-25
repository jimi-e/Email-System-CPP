#pragma once
#include "server_net.h"
#include <mysql/mysql.h>
#include <iostream>
#include <string>

class Identify : private server_base{
public:
	int check_user(int client_sck);
	int register_user(int client_sck);
private:
	char *server = "localhost";
	char *user = "root";
	char *password = "1234";
	char *database = "mail";
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
};


///客户端向服务端发送用户名与密码时，要以“/”分隔
///return 2**
int Identify::check_user(int client_sck){
	///connect mysql
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int info;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))	{
		fprintf(stderr,"%s\n",mysql_error(conn));
		return 2;
	}

	std::string msg1,msg2;
	std::string recv_str;
	receive_str(client_sck, recv_str);
	int length = recv_str.length();
	int i;
	for(i = 0;i < length;i++){
		if(recv_str[i] == '/')
		break;
	}
	msg1 = recv_str.substr(0,i);
	msg2 = recv_str.substr(i+1,length-i);
	std::cout<<msg1<<std::endl;
	std::cout<<msg2<<std::endl;

	///select in the mysql
	msg1 = "'" + msg1 + "'";
	std::string sql("select password from user where username =  "+msg1);
	std::cout<<sql<<std::endl;
	if(mysql_query(conn,sql.c_str()) != 0){
		printf("%s\n",mysql_error(conn));
		return -1;
	}else{
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		if(row == NULL)	{
			std::cout<<"There is no "<< msg1 << std::endl;
			send_str(client_sck,"wrong username");
			mysql_free_result(res);
			mysql_close(conn);
			return -1;//无该用户名
		}else{
			//printf("%s\n",row[0]);
			if(msg2==row[0]){
				printf("success password!\n");
				send_str(client_sck,"right");
				mysql_free_result(res);
				mysql_close(conn);
				return 0;
			}else{
				printf("error password!\n");
				send_str(client_sck,"wrong password");
				mysql_free_result(res);
				mysql_close(conn);
				return 1;
			}
		}  
	}
	return 2;
}
///客户端向服务端发送用户的注册信息，用户名和昵称和密码之间以“/”分隔
int Identify::register_user(int client_sck){
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		return 2;
	}
	///
	std::string recv_msg;
	///get message
	receive_str(client_sck, recv_msg);
	std::string username, nickname, pw;

	int length = recv_msg.length();
	int i;
	for(i = 0; i < length; i++){
		if(recv_msg[i] == '/')
		break;
	}
	username = recv_msg.substr(0,i);
	int j;
	i++;
	for(j = i; j < length; j++){
		if(recv_msg[j] == '/')
		break;
	}
	nickname = recv_msg.substr(i,j-i);
	pw = recv_msg.substr(j+1,length-j);
	///
	std::string sql = "INSERT INTO user (username,nickname,password) VALUES (";
	username = "'" + username + "'";
	nickname = "'" + nickname + "'";
	pw = "'" + pw + "'";
	sql = sql + username + ',' + nickname + ',' + pw + ')';
	std::cout<<sql<<std::endl;
	if(mysql_query(conn,sql.c_str()) != 0){
		printf("%s\n",mysql_error(conn));
		mysql_close(conn);
		send_str(client_sck,"register fail");
		return 1;
	}else{
		mysql_close(conn);
		send_str(client_sck,"register success");
		return 0;
	}
}