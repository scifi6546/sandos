#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#
const char *sudoers_path = "/etc/sudoers";
const char *passwd_path = "/etc/passwd";
char *read_string = "sandos";
int read_string_len = 6;
struct document{
	char *string;
	int length;
};
struct user{
	char *uname;
	char *passwd;
	int uuid;
	char *tmpuuid;
	int guid;
	char *tmpguid;
	char *homedir;
	char *shell;
	char *userinfo;
};
struct userarr{
	struct user *users;
	int length;
};
//declaring functions
struct document loadfile(char *file_path);
int finds(struct document doc, struct document find);
struct document make_sand_st(char *user, char *app);
struct document concat(struct document start,
		struct document end);
void f_write(char* file_path, struct document in);
struct userarr find_user(struct document passwd);
void analyze_users();
struct user mkusr();
void edit_sudo(char* user, char *app);
struct document mkpasswdst(struct userarr users);
char* alloc_string(char* one, char* two);
struct userarr set_ints(struct userarr users);
int gen_uuid(struct userarr users);
int gen_guid(struct userarr users);
struct userarr add_user(struct userarr users, char *uname, char* userinfo,
		char *homedir, char *shell);
//stop declaring functions

struct document sudoers;
struct document loadfile(char *file_path){
	char char_temp[2] = "1";
	struct document doc;
	int temp=0;
	FILE *file = fopen(file_path,"r");
	doc.length=10;
	doc.string = (char *)calloc(doc.length,sizeof(*doc.string));
	int len_doc_read = 0;
	while(temp != EOF){
		temp = fgetc(file);
		if(temp == EOF){
			 break;
		}
		char_temp[0] = (char) temp;
		if(doc.length - len_doc_read==1){
			doc.string = (char *)realloc(doc.string, (doc.length+10)*sizeof(*doc.string));
			doc.length+=10;
		}
		len_doc_read++;
		strcat(doc.string, char_temp);	
	}
	doc.string = realloc(doc.string,(doc.length+2)*(sizeof(*doc.string)));
	doc.length+=2;
	strcat(doc.string,"\0");
	if(file != NULL)
		fclose(file);// I do not why I have to remove
	//^^^^^
	//I am the cracker of C, destroyer of programming! --me
	if(doc.string == NULL)//note that I am an idiot, I put doc.string=NULL and asked a question on stack overflow
		printf("memory failed");
	doc.length=strlen(doc.string);
	return doc;
}
int finds(struct document doc,struct document find){ 
	int correct_letters=0;
	//measures number of correct leters
	//if there is a # than correct_letters=-1 
	for(int i =0; i<doc.length; i++){
		if(correct_letters==find.length-1)
			return 0;
		if(doc.string[i]=='#')
			correct_letters = -1;
		
		if(correct_letters != -1){
			if(doc.string[i]==find.string[correct_letters]){
				correct_letters++;
			}
		}
		if(doc.string[i]=='\n'){
			correct_letters = 0;
		}
	}
	return 1;
}
struct document make_sand_st(char* user, char *app){
	struct document ret;
	//argv[2]=application argv[1]=user
	ret.length = 150;
	char *out = calloc(ret.length,sizeof(char)*ret.length);
	strcat(out,"ALL=(sandos_");
	strcat(out,user);
	strcat(out,"_");
	strcat(out,app);
	strcat(out,") NOPASSWD: /usr/bin/");
	strcat(out,app);
	ret.string = out;
	ret.length=strlen(ret.string);
	return ret;
	//I am doing what is below in the awefull mess above
	//return "ALL=(sandos_" + argv[0] + "_" + argv[1] + ") NOPASSWD: /usr/bin" + argv[1];
}
struct document concat(struct document start, struct document end){
	int nlen = strlen(start.string)+ strlen(end.string) + 1;
	struct document out;
	out.string = calloc(nlen,sizeof(char));
	out.length = nlen;
	out.string = strcat(out.string,start.string);
	out.string = strcat(out.string, end.string);
	return out;
}
void f_write(char *file_path, struct document in){
	FILE *file = fopen(file_path, "w");
	fputs(in.string, file);
	fclose(file);
}
struct userarr find_user(struct document passwd){//this puts /etc/passwd info into users struct
	int status = 0;
	int read = 0;
	char temps[] = " ";
	struct userarr user_array;
	//if read = 0 do not put char in user, if read = 1 put in user
	struct user* users;
	users = calloc(1000000*10,sizeof(char));
	int length = 1;
	int usernum=0;
	int newline = 0;
	users[0]=mkusr();
	//status = -1 with comment, 0 means nothing happened, 
	//1 means on uname, 2 on uuid, 3 homedir, 4 shell
	for(int i = 0; i<passwd.length; i++){
		if(passwd.string[i]=='#')
			status=-1;	
		if(passwd.string[i]=='\n'){
			status = 0;
			newline=1;
			usernum++;
		}else{
			newline=0;
		}
		if(length-usernum<=1){
			for(int i = 0; i<10; i++)
				users[i+usernum] = mkusr();
			length+=10;
		}
		if(status>=0 && newline==0){
			if(passwd.string[i]==':'){
				read=0;
				status++;
			}else{
				temps[0] = passwd.string[i];
				if(status==0)
					strcat(users[usernum].uname,temps);
				if(status==1)
					strcat(users[usernum].passwd,temps);
				if(status==2)
					strcat(users[usernum].tmpuuid,temps);
				if(status==3)
					strcat(users[usernum].tmpguid,temps);
				if(status==4)
					strcat(users[usernum].userinfo,temps);
				if(status==5)
					strcat(users[usernum].homedir,temps);
				if(status==6)
					strcat(users[usernum].shell,temps);
			}
		}
	}
	user_array.users = users;
	user_array.length = usernum;
	user_array=set_ints(user_array);
	return user_array;
}
struct userarr set_ints(struct userarr users){
	for(int i =0; i<users.length; i++){
		users.users[i].uuid=atoi(users.users[i].tmpuuid);
		users.users[i].guid=atoi(users.users[i].tmpguid);
	}
	return users;
}
void analyze_users(char *user, char *app){
	struct document passwd;
	passwd = loadfile(passwd_path);
	struct userarr users;
	users = find_user(passwd);
	struct document doctemp = 
		mkpasswdst(users);
	puts(doctemp.string);
	int uname_len=strlen("sandos_") + strlen(user) + 1 + strlen(app);
	char *uname=calloc(uname_len,sizeof(char));
	uname=strcat(uname,"sandos_");
	uname=strcat(uname,user);
	uname=strcat(uname,"_");
	uname=strcat(uname,app);
	char* homedir = calloc(uname_len+strlen("/home/"),sizeof(char));
	homedir=strcat(homedir,"/home/");
	homedir=strcat(homedir,uname);
	char* shell = "/bin/bash";
	char* userinfo="sandos user";
	users=add_user(users,uname,userinfo,homedir,shell);
	struct document users_out = mkpasswdst(users);
	f_write(passwd_path,users_out);
}
struct user mkusr(){
	struct user out;
	out.uname=calloc(60,sizeof(char));
	out.homedir=calloc(60,sizeof(char));
	out.shell=calloc(60,sizeof(char));
	out.tmpuuid=calloc(60,sizeof(char));
	out.tmpguid=calloc(60,sizeof(char));
	out.passwd=calloc(60,sizeof(char));
	out.userinfo=calloc(60,sizeof(char));
	out.guid=0;
	out.uuid=0;
	return out;
}
struct document mkpasswdst(struct userarr users){
	struct document doc;
	doc.string = calloc(1000,sizeof(char));
	doc.length=1000;
	struct document temp;
	struct document semi;
	for(int i =0;i<users.length;i++){
		for(int j =0; j<=6;j++){
			if(j==0){
				temp.string=alloc_string(temp.string,users.users[i].uname);
				temp.length=strlen(temp.string);
			}
			if(j==1){
				temp.string=alloc_string(temp.string,users.users[i].uname);
				temp.length=strlen(temp.string);
			}
			if(j==2){
				temp.string=alloc_string(temp.string,users.users[i].tmpuuid);
				temp.length=strlen(temp.string);
			}
			if(j==3){
				temp.string=alloc_string(temp.string,users.users[i].tmpguid);
				temp.length=strlen(temp.string);
			}
			if(j==4){
				temp.string=alloc_string(temp.string,users.users[i].userinfo);
				temp.length=strlen(temp.string);
			}
			if(j==5){
				temp.string=alloc_string(temp.string,users.users[i].homedir);
				temp.length=strlen(temp.string);
			}
			if(j==6){
				temp.string=alloc_string(temp.string,users.users[i].shell);
				temp.length=strlen(temp.string);
			}
			doc = concat(doc,temp);
			if(j<6){
				doc.length+=2;
				doc.string = (char*) realloc(doc.string,doc.length*sizeof(char));
				doc.string= strcat(doc.string,":");
			}
			//doc.string=realloc(doc.string,doc.length*sizeof(char));
			//I edited make
		}
		doc.string=realloc(doc.string,(doc.length+2)*sizeof(char));
		doc.length+=2;
		strcat(doc.string,"\n");
	}
	return doc;
}
char *alloc_string(char* one, char* two){
	char* temp;
	temp=(char *)calloc(strlen(two)+1,sizeof(char));
	temp = strcpy(temp,two);
	return temp;
}
struct userarr add_user(struct userarr users,char* uname,char*  userinfo,char *homedir, char *shell){
	struct user temp = mkusr();
	temp.uname=strcpy(temp.uname,uname);	
	temp.userinfo=strcpy(temp.userinfo,userinfo);
	temp.homedir=strcpy(temp.homedir,homedir);
	temp.shell=strcpy(temp.shell,shell);
	temp.uuid = gen_uuid(users);
	temp.guid=gen_guid(users);
	sprintf(temp.tmpuuid,"%d",temp.uuid);
	sprintf(temp.tmpguid,"%d",temp.guid);
	users.users=realloc(users.users,(sizeof(temp)+30)*users.length);
	users.users[users.length]=temp;
	users.length++;
	//this adds new users to userarr
	return users;

}
int gen_uuid(struct userarr users){
	int uuid=1;
	int uuid_matches=0;
	for(int i = 1; i<10000;i++){
		uuid_matches=0;
		for(int j=0;j<users.length;j++){
			if(users.users[j].uuid==i)
				uuid_matches=1;
		}
		if(uuid_matches==0)
			return i;
	}
}

int gen_guid(struct userarr users){
	int guid=1;
	int guid_matches=0;
	for(int i=1;i<10000;i++){
		guid_matches=0;
		for(int j=0;j<users.length;j++){
			if(users.users[j].guid==i)
				guid_matches=1;	
		}
		if(guid_matches==0)
			return i;
	}
}
void edit_sudo(char *user,char *app){
	//I changed stuff make!
	struct document sudo = loadfile("thing");
	struct document sand=make_sand_st(user,app);
	int is_sand = finds(sudo, sand);
	if(is_sand == 0){//why do I forget to put 2 =
	       printf("already found I am stopping\n");
	}
	else{
		concat(sudo,sand);
		f_write(sudoers_path, sudo);
	}
}	
void edit_passwd(char *user, char *app){
	analyze_users(user,app);

}
void sandbox(char *user,char *app){
	edit_sudo(user,app);
	edit_passwd(user,app);
}
