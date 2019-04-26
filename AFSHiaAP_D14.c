#define FUSE_USE_VERSION 28

//gcc -pthread -Wall `pkg-config fuse --cflags` AFSHiaAP_D14.c -o AFSHiaAP.out `pkg-config fuse --libs`
//gcc -Wall `pkg-config fuse --cflags` AFSHiaAP_D14.c -o AFSHiaAP.out `pkg-config fuse --libs`

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/xattr.h>
#include <sys/param.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

pthread_t tid[10];

static const char *dirpath = "/home/irshadrasyidi/shift4";

//~ char customchar[] = {"qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};
char cesar[] = {"qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};


void enkrip(char message[]){
	int i, j;
	int karakter;
	for(i = 0; message[i] != '\0'; ++i){
		karakter = message[i];
		if(karakter == '/'){
			continue;
		}
		j = 0;
		while(j < strlen(cesar)){
			if(karakter == cesar[j]){
				break;
			}
			j++;
		}
		j += 17;
		if(j > 93){
			j -= strlen(cesar);
		}
		message[i] = cesar[j];
	}
}

void dekrip(char message[]){
	int i, j;
	int karakter;
	for(i = 0; message[i] != '\0'; ++i){
		karakter = message[i];
		if(karakter == '/'){
			continue;
		}
		j = 0;
		while(j < strlen(cesar)){
			if(karakter == cesar[j]){
				break;
			}
			j++;
		}
		j -= 17;
		if(j < 0){
			j += strlen(cesar);
		}
		message[i] = cesar[j];
	}
}

void listDir(const char* path){
	DIR* dir;
	struct dirent *ent;
	if((dir=opendir(path)) != NULL){
		while (( ent = readdir(dir)) != NULL){
			if(ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0  && strcmp(ent->d_name, "..") != 0){	
				char temp1[500];
				memset(&temp1, 0, sizeof(temp1));
				char tempa[500];
				memset(&tempa, 0, sizeof(tempa));
				char dst[500];
				memset(&dst, 0, sizeof(dst));
				
				
				strcat(temp1, path);
				strcat(temp1, "/");
				
				strcat(dst, path);
				strcat(dst, "/");
				strcat(dst, ent->d_name);
				
				strcat(tempa, ent->d_name);
				enkrip(tempa);
				
				strcat(temp1, tempa);
				
				printf("%s -> %s\n", dst, temp1);
				printf("%d\n\n", ent->d_type);
				
				rename(dst, temp1);
				
				listDir(temp1);
			}
			else{
				if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")){
					continue;
				}
				char temp2[500];
				memset(&temp2, 0, sizeof(temp2));
				char tempb[500];
				memset(&tempb, 0, sizeof(tempb));
				char dst2[500];
				memset(&dst2, 0, sizeof(dst2));
				
				strcat(temp2, path);
				strcat(temp2, "/");

				strcat(dst2, path);
				strcat(dst2, "/");
				strcat(dst2, ent->d_name);
				
				strcat(tempb, ent->d_name);
				enkrip(tempb);
				
				strcat(temp2, tempb);
				
				printf("%s -> %s\n", dst2, temp2);
				printf("%d\n\n", ent->d_type);
				
				rename(dst2, temp2);
			}
		}
		closedir(dir);
	}
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	
	char dir_last[500];
	memset(&dir_last, 0, sizeof(dir_last));
	
	char dir_en[500];
	memset(&dir_en, 0, sizeof(dir_en));
	
	strcpy(dir_en, path);
	//~ if(strcmp(dir_en, ".") != 0 && strcmp(dir_en, "..") != 0){
		//~ enkrip(dir_en);
	//~ }
	enkrip(dir_en);
	
	strcat(dir_last, dirpath);
	strcat(dir_last, dir_en);
	
	res = lstat(dir_last, stbuf);
	if (res == -1){
		return -errno;
	}

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			   off_t offset, struct fuse_file_info *fi)
{
	char dir_last[500];
	char dir_en[500];
	char dir_de[500];
	memset(&dir_last, 0, sizeof(dir_last));
	memset(&dir_en, 0, sizeof(dir_en));
	memset(&dir_de, 0, sizeof(dir_de));
	
	if(!strcmp(path, "/")){
		path = dirpath;
		sprintf(dir_last, "%s", path);
	}
	else{
		strcpy(dir_en, path);
		//~ if(strcmp(dir_en, ".") != 0 && strcmp(dir_en, "..") != 0){
			//~ enkrip(dir_en);
		//~ }
		enkrip(dir_en);
		memset(&dir_last, 0, sizeof(dir_last));
		strcat(dir_last, dirpath);
		strcat(dir_last, dir_en);
	}
	
	int res = 0;
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(dir_last);
	if (dp == NULL){
		return -errno;
	}
	
	while ((de = readdir(dp)) != NULL) {
		if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")){
			continue;
		}
		
		struct stat st;
		memset(&st, 0, sizeof(st));
		
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		
		memset(&dir_de, 0, sizeof(dir_de));
		strcpy(dir_de, de->d_name);

		dekrip(dir_de);
		
		res = (filler(buf, dir_de, &st, 0));
		if(res != 0){
			break;
		}
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi)
{
	
	char dir_last[500];
	char dir_en[500];
	memset(&dir_last, 0, sizeof(dir_last));
	memset(&dir_en, 0, sizeof(dir_en));
	
	if(!strcmp(path, "/")){
		path = dirpath;
		sprintf(dir_last, "%s", path);
	}
	else{
		strcpy(dir_en, path);
		//~ if(strcmp(dir_en, ".") != 0 && strcmp(dir_en, "..") != 0){
			//~ enkrip(dir_en);
		//~ }
		enkrip(dir_en);
		memset(&dir_last, 0, sizeof(dir_last));
		strcat(dir_last, dirpath);
		strcat(dir_last, dir_en);
	}
	int res = 0;
	int fd = 0;
	
	(void) fi;
	fd = open(dir_last, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{	
	//~ char tes[100];
	//~ memset(&tes, 0, sizeof(tes));
	//~ strcat(tes, "FAFAy");
	//~ printf("%s\n", tes);
	//~ enkrip(tes);
	//~ printf("%s\n", tes);
	//~ dekrip(tes);
	//~ printf("%s\n", tes);
	//~ dekrip(tes);
	//~ printf("%s\n", tes);
	
	//~ DIR *d;
	//~ struct dirent *dir;
	
	listDir(dirpath);
	//~ d = opendir(dirpath);
	//~ while ((dir = readdir(d)) != NULL) {
		//~ if((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0)){
			//~ continue;
		//~ }
		//~ else if(dir->d_type == 8){
			//~ printf("%d\n", dir->d_type);
			//~ printf("%s\n", dir->d_name);
			
		//~ }
		//~ else if(dir->d_type == 4){
			//~ printf("%d\n", dir->d_type);
			//~ printf("%s\n", dir->d_name);
			//~ encrypt(dir->d_name);
		//~ }
		
	//~ }
	//~ closedir(d);
	
	
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
