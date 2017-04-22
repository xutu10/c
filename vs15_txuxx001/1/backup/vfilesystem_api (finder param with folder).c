#include <vfilesystem_api.h>

/**
 * global variable for root folder
 */

Folder root; 

/**
 * find the position for the parameter referred folder
 * @param rfv - store the get back value
 * @return the parentID of target folder, 0 on error
 */

int32_t folderFinder(FolderID id, Folder* folder, returnFolderValue* rfv){

	Folder* currentFolder = NULL;	

	if(NULL == folder)
		return 0;

	currentFolder = folder-> subFolder;
	
	while(currentFolder != NULL){

		if(currentFolder -> folderID == id){				// if find the target
			strcp(rfv-> path,currentFolder->folderName);  
			strcat(rfv-> path,"/");							// store the path
			rfv-> folderPosition = currentFolder;				// store the position
			rfv-> id = currentFolder -> FolderID;
			return rfv-> id;
		}

		folderFinder(id, currentFolder,rfv);				// recursion to find the target
		if( 0 != rfv-> id){                               	// if find the target in subFolder
			strcat("/",rfv-> path);
			strcat(currentFolder -> folderName, rfv-> path);	 // joint the path	
			return rfv-> id;					 
		}

		currentFolder++;
	}
	return rfv-> id;
}

int32_t fileFinder(File id, returnFileVaule *rfv){

	File *currentFile 


}

FolderID fs_init() {

	int status = mkdir("root");

	if(!status){

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

