#include <vfilesystem_api.h>

/**
 * find the position for the parameter referred folder
 */

Folder* folderFinder(FolderID parent, Folder* folder){

	Folder* folderPointer = NULL;
	Folder* tmpPointer = NULL;

	if(NULL == folder)
		return folderPointer;

	folderPointer = folder-> subFolder;
	
	while(folderPointer != NULL){

		if(folderPointer -> folderID == parent)				// if find the result, return soon
			return folderPointer;

		tmpPointer = folderFinder(parent, folderPointer);
		if( NULL != tmpPointer){                             // if find the result in subFolder
			folderPointer = tmpPointer;						 // assign the folderPointer
			return folderPointer;							 // then return the result pointer
		}

		folderPointer++;
	}

	return folderPointer;

}

FolderID fs_init() {

	int status = mkdir("root");

	if(!status){

		Folder root;
		root.folderID = 1;
		root.folderName = "root";
		root.subFile = NULL;
		root.subFolder = NULL;
		root.fileCount = 0;
		root.folderCount = 0;
		root.next = NULL;
		printf("create root directory success\n");

		return root.folderID;
	}

	return INVALID_HANDLE;
}

void fs_destroy() {

	int status = rmdir("root");

	memset()


}

FileID fs_new_file(char *name, FolderID parent){

	return 0;
}

FolderID fs_new_folder(char *name, FolderID parent){

	return 0;
}

int32_t fs_delete_file(FileID file) {
	return 0;
}

int32_t fs_delete_folder(FolderID folder) {
	return 0;
}

FolderID fs_get_file_parent(FileID file) {
	return 0;
}

int32_t fs_get_file_size(FileID file) {
	return 0;
}

int32_t fs_get_file_name_length(FileID file) {
	return 0;
}

int32_t fs_get_file_name(FileID file, char* file_name, uint8_t max_length) {
	return 0;
}

FolderID fs_get_folder_parent(FolderID folder) {
	return 0;
}

int32_t fs_get_folder_name_length(FolderID folder) {
	return 0;
}

int32_t fs_get_folder_name(FolderID folder, char* folder_name, uint32_t max_length) {
	return 0;
}

int32_t fs_get_folder_file_count(FolderID folder) {
	return 0;
}

int32_t fs_get_folder_folder_count(FolderID folder) {
	return 0;
}

int32_t fs_get_folder_files(FolderID folder, FileID* files, uint32_t max_num_files) {
	return 0;
}

int32_t fs_get_folder_folders(FolderID folder, FolderID* folders, uint32_t max_num_folders) {
	return 0;
}

