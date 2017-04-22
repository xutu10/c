#include <stdio.h>
#include <string.h>
#include "../include/vfilesystem_api.h"

int main(int argc, char **args) {
	FolderID root = fs_init();

	// Your test code goes here
	FolderID folder1 = fs_new_folder("folder1", root);
	FolderID folder2 = fs_new_folder("folder2", root);
	FileID file1 = fs_new_file("file1", root);
	FolderID folder3 = fs_new_folder("folder3", root);
	FolderID folder4 = fs_new_folder("folder4", root);
	FolderID folder5 = fs_new_folder("folder5", folder1);
	FileID file2 = fs_new_file("file2", folder1);
	FileID file3 = fs_new_file("file3", folder1);
	FileID file4 = fs_new_file("file4", folder2);
	FileID file5 = fs_new_file("file5", folder3);
	FolderID folder6 = fs_new_folder("folder6", folder3);
	FileID file6 = fs_new_file("file6", folder3);
	FileID file7 = fs_new_file("file7", folder4);
	FileID file8 = fs_new_file("file8", folder4);
	FileID files[10];
	FolderID folders[10];
	memset(files, 0, sizeof(files));
	memset(folders, 0, sizeof(folders));

	fs_get_folder_files(root, files, sizeof(files)/sizeof(FileID));
	printf ("There are %d files in the root directory (root has id %d) wih IDs: ", fs_get_folder_file_count(root), root);
	unsigned long i;
	for(i = 0; i < sizeof(files)/sizeof(FileID); i++) {
		if(files[i] > 0)
			printf ("%d ",files[i]);
	}
	printf ("\n");
	fs_get_folder_folders(root, folders, sizeof(folders)/sizeof(FolderID));
	printf ("There are %d folders in the root directory (root has id %d) with IDs: ", fs_get_folder_folder_count(root), root);
	for(i = 0; i < sizeof(folders)/sizeof(FolderID); i++) {
		if(folders[i] > 0)
			printf ("%d ",folders[i]);
	}
	printf ("\n");
	printf ("Parent of folder with id %d is %d\n", folder2, fs_get_folder_parent(folder2));
	char name[10];
	fs_get_folder_name(folder4, name, sizeof(name));
	printf ("Folder with id %d has name %s\n", folder4, name);
	printf ("Length of that name is %d\n", fs_get_folder_name_length(folder4));
	printf ("Folder with id %d has %d files\n", folder4, fs_get_folder_file_count(folder4));
	printf ("Destroying one of those files....\n");
	fs_delete_file(file7);
	printf ("New number of files in folder with id %d is %d\n", folder4, fs_get_folder_file_count(folder4));
	printf ("Writing hello world into file with id %d...\n", file3);
	uint8_t data[] = "hello world";
	if(fs_write_file(file3, 0, sizeof(data)/sizeof(uint8_t), data) == 0) {
		printf ("Wrote data successfully\n");
		memset(data, 0, sizeof(data));
		fs_read_file(file3, 0, sizeof(data)/sizeof(uint8_t), data);
		printf ("Data in file with id %d now is ", file3);
		for(i = 0; i < fs_get_file_size(file3); i++) {
			printf ("%c ", data[i]);
		}
		printf ("\n");
	}
	else
		printf ("Writing failed!\n");
	printf ("Writing carlos with offset of 6\n");
	uint8_t newData[] = "carlos";
	if(fs_write_file(file3, 6, sizeof(newData), newData) == 0) {
		printf ("Wrote data successfully\n");
		uint8_t* result = malloc(fs_get_file_size(file3));
		fs_read_file(file3, 0, fs_get_file_size(file3), result);
		printf ("Data in file with id %d now is ", file3);
		for(i = 0; i < fs_get_file_size(file3); i++) {
			printf ("%c ", result[i]);
		}
		printf ("\n");
		free(result);
	}
	else
		printf ("Writing failed!\n");
	fs_destroy();
	return 0;
}
