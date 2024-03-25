#pragma once
#include "server_net.h"
#include <mysql/mysql.h>
#include <string>
#include <vector>

class Mail : server_base{
public:
	int send_text(int sock);
	int send_attach(int sock);
	int send_list(int sock);
	int receive_mail(int client_sck);
	int delete_mail(int sock);
private:
	int info;
	char *server = "localhost";
	char *user = "root";
	char *password = "1234";
	char *database = "mail";
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
};

int Mail::send_text(int sock){
	std::string text;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		return 2;
	}
	
	std::string ID;
	receive_str(sock, ID);
	std::string sql("select text,state from letters where id="+ID+";");
	std::cout<<sql<<std::endl;
	if(mysql_query(conn, sql.c_str())){
		printf("sql err!\n");
		return 2;
	}
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	if(row){
		text = row[0];
	}else{
		send_str(sock, "get fail");
		mysql_free_result(res);
		mysql_close(conn);
		return -1;
	}
	if(row[1][0]=='0'){
		sql = "update letters set state=1 where id="+ID+";";
		if(mysql_query(conn, sql.c_str())){
			printf("sql err!\n");
			return 2;
		}
	}else if(row[1][0]=='2'){
		sql = "delete from letters where id="+ID+";";
		if(mysql_query(conn, sql.c_str())){
			printf("sql err!\n");
			return 2;
		}
	}
	send_str(sock, "get success");
	send_str(sock, text.c_str());
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}
///return -1
int Mail::send_attach(int sock){
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		return -1;
	}
	std::string ID;
	receive_str(sock, ID);
	std::string sql("select attachment from letters where id="+ID);
	std::cout<<sql<<std::endl;
	if(mysql_query(conn, sql.c_str())){
		printf("sql err!\n");
		return -1;
	}
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	std::vector<std::string> list;
	int length = strlen(row[0]);
	int num = 0;
	std::string filename;
	if(length!=0){
		for(int i=0;i<length;i++){
			if(row[0][i]=='/'){
				list.push_back(filename);
				filename.clear();
				num++;
			}else{
				filename+=row[0][i];
			}
		}
		list.push_back(filename);
		filename.clear();
		num++;
	}
	std::string path("./attachment/");
	send_str(sock, std::to_string(num).c_str());
	for(int i = 0; i<num; i++){
		send_str(sock, list[i].c_str());
		std::cout<<list[i]<<std::endl;
		if(send_file(sock, (path+list[i]).c_str())<0){
			return -1;
		}
	}
	std::cout<<"send success "<<num<<std::endl;
	mysql_free_result(res);
	mysql_close(conn);
	return num;
}
///return -1
int Mail::send_list(int sock){
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		return -1;
	}
	std::string name, type;
	receive_str(sock, name);
	receive_str(sock, type);
	std::string sql("select id,sender,receiver,theme,time,state from letters where ");
	if(type[0]!='0') sql+="sender='"+name+"' and (state=";
	else sql+="receiver='"+name+"' and (state=";
	switch (type[0]){
	case '0':
	case '1':
		sql += "0 or state = 1)";
		break;
	case '2':
		sql += "2)";
		break;
	case '3':
		sql += "3)";
		break;
	}
	std::cout<<sql<<std::endl;
	if(mysql_query(conn, sql.c_str())){
		printf("sql err! %s\n", mysql_error(conn));
		return -1;
	}
	res = mysql_store_result(conn);
	std::string send_bag;
	int num = mysql_num_fields(res), n = 0;
	while((row = mysql_fetch_row(res))!=NULL){
		n++;
		send_bag.clear();
		for(int i=0;i<num;i++){
			if(i) send_bag += '/';
			send_bag += row[i];
		}
		send_str(sock, send_bag.c_str());
	}
	send_str(sock, "sendend");
	mysql_free_result(res);
	mysql_close(conn);
	std::cout<<"mail mount:"<<n<<std::endl;
	return n;
}

int Mail::receive_mail(int client_sck){
    ///获取附件名字串
	
	std::string temp;
	std::string name;
	receive_str(client_sck,temp);
	int num = atoi(temp.c_str());
	//name存放附件名字，中间以'/'分隔
	int x = 1;
	while(num--){
		receive_file(client_sck,"attachment");
		receive_str(client_sck,temp);
		if(x == 1){
			name += temp;
			x++;
		}else{
			name = name + "/" + temp;
		}
	}
    std::cout << name << std::endl;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		return 2;
	}

	std::string recv_info;
	receive_str(client_sck,recv_info);

	///从获取到的邮件信息中分离出收件人、发送人、邮件主题
	std::string rece_info,sen_info,title;
	char state = recv_info[0];
	int length = recv_info.length();
	int i;
	for(i = 2;i < length;i++){
		if(recv_info[i] == '/')
		break;
	}
	rece_info = recv_info.substr(2,i-2);
	int j;
	i++;
	for(j = i;j < length;j++){
		if(recv_info[j] == '/')
		break;
	}
	sen_info = recv_info.substr(i,j-i);
	title = recv_info.substr(j+1,length-j);
	length = title.length();
	char *tmp = new char[length*2+1];
	if(!tmp){
		printf("new err!\n");
		return 2;
	}
	mysql_real_escape_string(conn, tmp, title.c_str(), length);

	std::string sql = "INSERT INTO letters (receiver,sender,theme,text,state,attachment) VALUES (";
	rece_info=  "'" + rece_info + "'";
	sen_info =  "'" + sen_info  + "'";
	title    =  "'" + std::string(tmp)     + "'";
	sql = sql + rece_info + ',' + sen_info + ',' + title + ',';
	delete[] tmp;
	std::string recv_str;
	receive_str(client_sck, recv_str);
	length = recv_str.length();
	tmp = new char[length*2+1];
	if(!tmp){
		printf("new err!\n");
		return 2;
	}
	mysql_real_escape_string(conn, tmp, recv_str.c_str(), length);
	sql += "'";
	sql += tmp;
	sql += "'";
	if(state=='0') sql += ", 0";
	else sql+=", 2";
	delete[] tmp;
     
	 sql = sql + "," + "'" + name + "'" + ')';
	std::cout<<sql<<std::endl;
	if(mysql_query(conn,sql.c_str())!=0){
		printf("%s\n",mysql_error(conn));
		send_str(client_sck,"send fail!");
		return 1;
	}else{
		send_str(client_sck,"send success!");
		return 0;
	}
	mysql_free_result(res);
	mysql_close(conn);
}

int Mail::delete_mail(int sock){
	std::string ID;
	receive_str(sock, ID);
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		return 2;
	}
	std::string sql("update letters set state=3 where id = "+ID);
	std::cout<<sql<<std::endl;
	if(mysql_query(conn, sql.c_str())!=0){
		printf("%s", mysql_error(conn));
		send_str(sock, "delete fail");
		mysql_close(conn);
		return 1;
	}else{
		send_str(sock, "delete success");
		mysql_close(conn);
		return 0;
	}
}
