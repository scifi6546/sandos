#ifndef LIBSANDOS.H
#define LIBSANDOS.H
struct document{
	char *string;
	int length;
};
struct user{
	char *uname;
	char *passwd;
	char uuid;
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
void sandbox(char*user, char *app);
void remove_sandbox(char *user, char *app);
#endif
