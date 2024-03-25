#ifndef MAIL_H
#define MAIL_H
#pragma once
#include "client_net.h"
//#include "attach.h"
#include <string>
#include <vector>

struct Mail_head {
    char state[25];
    char ID[25];
    char time[25];
    char sender[50];
    char recver[50];
    char title[50];
};

class Mail {
public:
    std::string sender() const { return sender_; }
    std::string recver()const { return recver_; }
    std::string title()const { return title_; }
    std::string send_time() const { return send_time_; }
    std::string text()const { return text_; }
private:
protected:
    std::string ID;
    std::string sender_;
    std::string recver_;
    std::string title_;
    std::string send_time_;
    std::string text_;
    int attach_num;
    //std::vector<Attachment>  attach_list;
};

/****/

class Mail_recv : public Mail, private client_base {
public:
    Mail_recv() {}
    Mail_recv(int sock, std::string ID) :sock(sock) { set_ID(ID); }
    int sock;
    void set_ID(const std::string& s) { ID = s; }
    void set_sock(int s) { sock = s; }
    void set_sender(std::string s) { sender_ = s; }
    void set_recver(std::string s) { recver_ = s; }
    void set_title(std::string s) { title_ = s; }
    void set_time(std::string s) { send_time_ = s; }
    int get_text();
    int get_attach(const char* path, std::vector<std::string>& name_list);
private:

};

inline int Mail_recv::get_text() {
    if (ID.empty()) return -1;
    send_str(sock, "gettext");
    send_str(sock, ID.c_str());
    text_.clear();
    std::string recv_str;
    receive_str(sock, recv_str);
    if (recv_str == "get fail") {
        return -1;
    }
    receive_str(sock, text_);
    if (text_.empty()) return 1;
    else return 0;
}

inline int Mail_recv::get_attach(const char* path, std::vector<std::string>& name_list) {
    if (ID.empty()) return -1;
    send_str(sock, "getattach");
    send_str(sock, ID.c_str());
    std::string recv_buf;
    receive_str(sock, recv_buf);
    int num = atoi(recv_buf.c_str());
    for (int i = 0; i < num; i++) {
        receive_str(sock, recv_buf);
        name_list.push_back(recv_buf);
        receive_file(sock, path);
    }
    return num;
}

/****/

class Mail_send : public Mail, private client_base {
public:
    int set_sender(std::string s);
    int set_recver(std::string s);
    int set_title(std::string s);
    int set_text(std::string s);
    int to_send_mail(int client_sck, const char* info, const char* text, int state = 0, std::vector<std::string>* vec = NULL);
    int delete_mail(int sock, std::string ID);
private:

};

///info为“收件人用户名/发件人用户名/主题“
///text为要发送的文本内容
///函数返回值为0代表发送成功，返回值为1代表发送失败，返回值为2代表未受到反馈信息
///state为0是发送邮件，1为保存草稿
///vec 附件路径 默认为NULL
inline int Mail_send::to_send_mail(int client_sck, const char* info, const char* text, int state, std::vector<std::string>* vec) {
    send_str(client_sck, "sendmail");

    int size = vec->size();
    send_str(client_sck, std::to_string(size).c_str());
    int i = 0;
    while (size--) {
        send_file(client_sck, (*vec)[i].c_str());
        send_str(client_sck, get_filename((*vec)[i].c_str()).c_str());
        std::cout << get_filename((*vec)[i].c_str()) << std::endl;
        i++;
    }

    std::string recv_buf;
    //std::string str;
    send_str(client_sck, (std::to_string(state) + '/' + info).c_str());
    send_str(client_sck, text);
    receive_str(client_sck, recv_buf);
    if (recv_buf == "send fail!") {
        return 1;
    }
    else if (recv_buf == "send success!") {
        return 0;
    }
    else {
        return 2;
    }
}

inline int Mail_send::delete_mail(int sock, std::string ID) {
    send_str(sock, "deletemail");
    send_str(sock, ID.c_str());
    std::string recv_str;
    receive_str(sock, recv_str);
    if (recv_str == "delete fail") {
        return 1;
    }
    else {
        return 0;
    }
}
/****/

class Mail_recv_list : private client_base {
public:
    Mail_recv_list(bool is = true) : is_sender(is) { }
    Mail_recv_list(int sock, std::string n) :sock(sock), name(n) {}
    int sock;
    void set_sock(int s) { sock = s; }
    void set_name(std::string s) { name = s; }
    void set_is_sender(bool is) { is_sender = is; }
    int get_mail(std::vector<Mail_head>& list, int type = 0);
private:
    bool is_sender;
    std::string name;
};

//接受
//type：0表示收件箱 1表示发件箱 2表示草稿 3表示垃圾箱
inline int  Mail_recv_list::get_mail(std::vector<Mail_head>& list, int type) {
    if (name.empty()) return -1;
    send_str(sock, "getlist");
    send_str(sock, name.c_str());
    send_str(sock, std::to_string(type).c_str());
    std::string recv_buf;
    list.clear();
    Mail_head tmp;
    while (1) {
        receive_str(sock, recv_buf);
        memset(&tmp, 0, sizeof(tmp));
        if (recv_buf == "sendend") break;
        int len = recv_buf.length();
        for (int i = 0, x = 0, j = 0; i < len; i++) {
            if (recv_buf[i] == '/') {
                x++;
                j = 0;
            }
            else switch (x) {
            case 0: {
                tmp.ID[j++] = recv_buf[i];
                break;
            }
            case 1: {
                tmp.sender[j++] = recv_buf[i];
                break;
            }
            case 2: {
                tmp.recver[j++] = recv_buf[i];
                break;
            }
            case 3: {
                tmp.title[j++] = recv_buf[i];
                break;
            }
            case 4: {
                tmp.time[j++] = recv_buf[i];
                break;
            }
            case 5: {
                tmp.state[j++] = recv_buf[i];
                break;
            }
            }
        }
        list.push_back(tmp);
    }
    return list.size();
}


#endif // MAIL_H
