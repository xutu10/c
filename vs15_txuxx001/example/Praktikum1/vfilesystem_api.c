#include "../include/vfilesystem_api.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//this struct represents both folders and files
//I use the term item to refer to both in the comments
struct Item {
    int32_t id;
    char* name;
    bool isFolder;
    struct Item* father;
    struct Item* previousSibling;
	struct Item* nextSibling;
    struct Item* child;
    uint8_t* data;
	uint32_t size;
};
struct Item* rootDirectory;
int32_t idPool = 0;

FolderID fs_init() {
	rootDirectory = malloc(sizeof(struct Item));
    rootDirectory->id = idPool++;
    rootDirectory->name = malloc(strlen("root") + 1);
	strcpy(rootDirectory->name, "root");
    rootDirectory->isFolder = true;
    rootDirectory->father = NULL;
    rootDirectory->previousSibling = NULL;
	rootDirectory->nextSibling = NULL;
    rootDirectory->child = NULL;
    rootDirectory->data = NULL;
	rootDirectory->size = 0;
    return 0;
}

/**
   Helper function to locate folders or files by ID starting at pivot
**/
struct Item* findItem(int32_t id, struct Item* pivot) {
    if(pivot == NULL)
        return NULL;
    struct Item* index = pivot;
    if(index->id == id) {
        //item found
        return index;
	}
	//not in the right folder so search one level deeper
    index = findItem(id, pivot->child);
    if(index == NULL) {
		//couldn't find it, search across
        index = findItem(id, pivot->nextSibling);
	}
    return index;
}

FileID fs_new_file(char* name, FolderID parent) {
    if (parent < 0) {
        return INVALID_HANDLE;
	}
	if (name == NULL) {
		return -1;
	}
	//find the parent folder
    struct Item* parentFolder =  findItem(parent, rootDirectory);
    if(parentFolder == NULL) {
		return INVALID_HANDLE;
	}
	if(parentFolder->child == NULL) {
		//parentFolder is an empty folder
		parentFolder->child = malloc(sizeof(struct Item));
		parentFolder->child->id = idPool++;
		parentFolder->child->name = malloc(strlen(name) + 1);
		strcpy(parentFolder->child->name, name);
		parentFolder->child->isFolder = false;
		parentFolder->child->father = parentFolder;
		parentFolder->child->previousSibling = NULL;
		parentFolder->child->nextSibling = NULL;
		parentFolder->child->child = NULL;
		parentFolder->child->data = NULL;
		parentFolder->child->size = 0;
		return parentFolder->child->id;
	}
	struct Item* index = parentFolder->child;
	//search across the folder to determine whether an item with the same name already exists
	while(index->nextSibling != NULL) {
		if(strcmp(index->name, name) == 0) {
			// if it exists return an error
			return -2;
		}
		index = index->nextSibling;
	}
	//check if last item has the same name
	if(strcmp(index->name, name) == 0)
		return -2;
	//while loop exited normally: there's no item with the name provided
	index->nextSibling = malloc(sizeof(struct Item));
	index->nextSibling->id = idPool++;
	index->nextSibling->name = malloc(strlen(name) + 1);
	strcpy(index->nextSibling->name, name);
	index->nextSibling->isFolder = false;
	index->nextSibling->father = parentFolder;
	index->nextSibling->nextSibling = NULL;
	index->nextSibling->previousSibling = index;
	index->nextSibling->child = NULL;
	index->nextSibling->data = NULL;
	index->nextSibling->size = 0;
	return index->nextSibling->id;
}

FolderID fs_new_folder(char* name, FolderID parent) {
	if(parent < 0) {
		return INVALID_HANDLE;
	}
	if(name == NULL) {
		return -1;
	}
	struct Item* parentFolder = findItem(parent, rootDirectory);
	if(parentFolder == NULL)
		return INVALID_HANDLE;
	if(parentFolder->child == NULL) {
		//parent folder is an empty folder
		parentFolder->child = malloc(sizeof(struct Item));
		parentFolder->child->id = idPool++;
		parentFolder->child->name = malloc(strlen(name) + 1);
		strcpy(parentFolder->child->name, name);
		parentFolder->child->isFolder = true;
		parentFolder->child->father = parentFolder;
		parentFolder->child->nextSibling = NULL;
		parentFolder->child->previousSibling = NULL;
		parentFolder->child->child = NULL;
		parentFolder->child->data = NULL;
		parentFolder->child->size = 0;
		return parentFolder->child->id;
	}
	struct Item* index = parentFolder->child;
	//search across the folder to determine whether an item with the same name already exists	
	while(index->nextSibling != NULL) {
		if(strcmp(index->name, name) == 0) {
			return -2;
		}
		index = index->nextSibling;
	}
	//check if last item has the same name
	if(strcmp(index->name, name) == 0)
		return -2;
	//while loop exited normally: there's no file with the name provided
	index->nextSibling = malloc(sizeof(struct Item));
	index->nextSibling->id = idPool++;
	index->nextSibling->name = malloc(strlen(name) + 1);
	strcpy(index->nextSibling->name, name);
	index->nextSibling->isFolder = true;
	index->nextSibling->father = parentFolder;
	index->nextSibling->nextSibling = NULL;
	index->nextSibling->previousSibling = index;
	index->nextSibling->child = NULL;
	index->nextSibling->data = NULL;
	return index->nextSibling->id;
}
/**
   Helper function that deletes everything
   to the right of the current level and below it
**/
void destroyRecursively(struct Item* pivot) {
	if(pivot == NULL)
		return;
	destroyRecursively(pivot->child);
	destroyRecursively(pivot->nextSibling);
	if(pivot->data != NULL)
		free(pivot->data);
	free(pivot->name);
   	free(pivot);
}

void fs_destroy() {
	destroyRecursively(rootDirectory);
}

int32_t fs_delete_file(FileID file) {
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder)
		return -1;
	if(handler->nextSibling == NULL && handler->previousSibling == NULL) {
		//we're deleting the only item in the folder
		handler->father->child = NULL;
	}
	else if (handler->previousSibling == NULL && handler->nextSibling != NULL) {
		//there's one more item in the folder after this one
		handler->nextSibling->previousSibling = NULL;
		handler->father->child = handler->nextSibling;
	}
	else if (handler->previousSibling != NULL && handler->nextSibling == NULL) {
		//there's one more item in the folder before this one
		handler->previousSibling->nextSibling = NULL;
	}
	else {
		//there are items both before and after this file
		handler->previousSibling->nextSibling = handler->nextSibling;
		handler->nextSibling->previousSibling = handler->previousSibling;
	}
	if(handler->data != NULL)
		free(handler->data);
	free(handler->name);
	free(handler);
    return 0;
}

int32_t fs_delete_folder(FolderID folder) {
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	if(handler->id == 0)
		//can't delete the root directory, fs_destroy should be called for that
		return -2;
	if(handler->nextSibling == NULL && handler->previousSibling == NULL) {
		//we're deleting the only item in the folder
		handler->father->child = NULL;
	}
	else if (handler->previousSibling == NULL && handler->nextSibling != NULL) {
		//there's one more item in the folder after this one
		handler->nextSibling->previousSibling = NULL;
		handler->father->child = handler->nextSibling;
	}
	else if (handler->previousSibling != NULL && handler->nextSibling == NULL) {
		//there's one more item in the folder before this one
		handler->previousSibling->nextSibling = NULL;
	}
	else {
		//there are items both before and after this file
		handler->previousSibling->nextSibling = handler->nextSibling;
		handler->nextSibling->previousSibling = handler->previousSibling;
	}
	destroyRecursively(handler->child);
	free(handler->name);
	free(handler);
    return 0;
}

FolderID fs_get_file_parent(FileID file) {
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder)
		return -1;
	return handler->father->id;
}

FolderID fs_get_folder_parent(FolderID folder) {
	if(folder == 0)
		return 0; //changed this to use it on fileserver, was -2
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	return handler->father->id;
}

int32_t fs_get_file_size(FileID file) {
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder)
		return -1;
	return handler->size;
}

int32_t fs_get_file_name_length(FileID file) {
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder)
		return -1;
	return strlen(handler->name);
}

int32_t fs_get_folder_name_length(FolderID folder) {
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	return strlen(handler->name);
}

int32_t fs_get_file_name(FileID file, char* file_name, uint8_t max_length) {
	if(file_name == NULL)
		return -2;
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder)
		return -1;
	strncpy(file_name, handler->name, max_length);
    return 0;
}

int32_t fs_get_folder_name(FolderID folder, char* folder_name, uint32_t max_length) {
	if(folder_name == NULL)
		return -2;
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	strncpy(folder_name, handler->name, max_length);
    return 0;
}

int32_t fs_get_folder_file_count(FolderID folder) {
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	if(handler->child == NULL)
		return 0;
	int32_t counter = 0;
	handler = handler->child;
	if(!handler->isFolder)
		counter++;
	while(handler->nextSibling != NULL) {
		handler = handler->nextSibling;
		if(!handler->isFolder)
			counter++;
	}
	return counter;
}

int32_t fs_get_folder_folder_count(FolderID folder) {
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	if(handler->child == NULL)
		return 0;
	int32_t counter = 0;
	handler = handler->child;
	if(handler->isFolder)
		counter++;
	while(handler->nextSibling != NULL) {
		handler = handler->nextSibling;
		if(handler->isFolder)
			counter++;
	}
	return counter;
}

int32_t fs_get_folder_files(FolderID folder, FileID* files, uint32_t max_num_files) {
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	if(handler->child == NULL)
		return 0;
	handler = handler->child;
	uint32_t counter = 0;
	while(handler != NULL) {
		if(!handler->isFolder) {
			files[counter] = handler->id;
			counter++;
		}
		if(counter == max_num_files)
			break;
		handler = handler->nextSibling;
	}
	return 0;
}

int32_t fs_get_folder_folders(FolderID folder, FolderID* folders, uint32_t max_num_folders) {
	struct Item* handler = findItem(folder, rootDirectory);
	if(handler == NULL || !handler->isFolder)
		return -1;
	if(handler->child == NULL)
		return 0;
	handler = handler->child;
	uint32_t counter = 0;
	while(handler != NULL) {
		if(handler->isFolder) {
			folders[counter] = handler->id;
			counter++;
		}
		if(counter == max_num_folders)
			break;
		handler = handler->nextSibling;
	}
	return 0;
}

int32_t fs_write_file(FileID file, u_int32_t offset, u_int32_t length, u_int8_t* data) {
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder)
		return -1;
	void* tempPtr = realloc(handler->data, length + offset);
	if(tempPtr == NULL)
		//memory reallocation failed
		return -1;
	else
		handler->data = tempPtr;
	memset(handler->data, 0, length + offset); //new change for prak 4
	handler->size = length + offset;
	memcpy(handler->data + offset, data, length);
	return 0;
}

int32_t fs_read_file(FileID file, u_int32_t offset, u_int32_t length, u_int8_t* data) {
	struct Item* handler = findItem(file, rootDirectory);
	if(handler == NULL || handler->isFolder || handler->data == NULL)
		return -1;
	if(length + offset > handler->size)
		return -1;
	memcpy(data, handler->data + offset, length);
	return 0;
}
