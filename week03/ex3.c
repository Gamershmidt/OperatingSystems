#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PATH 2048
#define MAX_FNAME 64
#define MAX_SIZE 1024
struct File;

struct Directory {
	char name[64];
	unsigned char nf;
	unsigned char nd;
	char path[MAX_PATH];
	struct File *files[MAX_FNAME];
	struct Directory* directories[MAX_SIZE];
};
struct File {
	int id;
	char name[MAX_FNAME];
	int size;
	char data[MAX_SIZE];
	struct Directory directory;
};


// Prints the name of the File file
void show_file(struct File *file){
	printf("%s", file->name);
}
// Displays the content of the Directory dir
void show_dir(struct Directory *dir) {
	printf("\nDIRECTORY\n");
	printf(" path: %s\n", dir->path);
	printf(" files:\n");
	printf(" [ ");
	for (int i = 0; i < dir->nf; i++){
		show_file(dir->files[i]);
	}			
	printf("]\n");
	printf(" directories:\n");
	printf(" { ");
	for (int i = 0; i < dir->nd; i++){
		show_dir(dir->directories[i]);
	}	
	printf("}\n");
}

void add_dir(struct Directory *dir1,struct Directory *dir2) {
	if (dir1 && dir2) {
		dir2->directories[dir2->nd] = dir1;
		dir2->nd++;
		char temp_path[MAX_PATH];
		if (strcmp(dir2->path, "/")) {
			strcpy(temp_path, dir2->path);
			strcat(temp_path, "/");
			strcat(temp_path, dir1->name);
			strcpy(dir1->path, temp_path);
		} else {
			strcpy(temp_path, "/");
			strcat(temp_path, dir1->name);
			strcpy(dir1->path, temp_path);
		}
	}
}

void add_file(struct File* file, struct Directory* dir) {
	dir->files[dir->nf] = file;
	file->directory = *dir;
	dir->nf++;
}
void overwrite_to_file(struct File* file, const char* str) {
	strncpy(file->data,str, sizeof(file->data));
}
void append_to_file(struct File* file, const char* str){
	strncat(file->data, str, 1024 - sizeof(file->data));
}
void printp_file(struct File* file){
	printf("%s/", file->directory.path);
	show_file(file);
	printf("\n");	
}

int main() {
	struct Directory* root = malloc(sizeof(struct Directory));
	root->name[0] = '/';
	root->nf = 0;
	root->nd = 0;
	struct Directory* bin = malloc(sizeof(struct Directory));
	strncpy(bin->name, "bin", 3);
	bin->nf = 0;
	bin->nd = 0;
	struct Directory* home = malloc(sizeof(struct Directory));
	strncpy(home->name, "home", 4);
	home->nf = 0;
	home->nd = 0;
	add_dir(bin, root);
	add_dir(home, root);
	struct File* bash = malloc(sizeof(struct File));
	bash->id = 0;
	bash->size = 256;
	strncpy(bash->name, "bash", 4);
	struct File* ex3_1 = malloc(sizeof(struct File));
	ex3_1->id = 1;
	ex3_1->size = 256;
	strncpy(ex3_1->name, "ex3_1.c", 7);
	overwrite_to_file(ex3_1, "int printf(const char * format, ...)");
	struct File* ex3_2= malloc(sizeof(struct File));
	ex3_2->id = 2;
	ex3_2->size = 256;
	strncpy(ex3_2->name, "ex3_2.c", 7);
	overwrite_to_file(ex3_2, "“//This is a comment in C language");
	append_to_file(ex3_1, "int main(){printf(”Hello World!”)}");
	add_file(bash, bin);
	add_file(ex3_1, home);
	add_file(ex3_2, home);
	overwrite_to_file(bash, "Bourne Again Shell!");
	printp_file(bash);
	printp_file(ex3_1);
	printp_file(ex3_2);
	free(bin);
	free(root);
	free(home);
	free(bash);
	free(ex3_1);
	free(ex3_2);
	return 0;
}
