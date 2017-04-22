#ifndef FILESERVERAPI_H_
#define FILESERVERAPI_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef int32_t FileID;
typedef int32_t FolderID;

#define INVALID_HANDLE -100

/**
 * create struct file und file list.
 */
struct file{

	FileID fileID;
	char *fileName;
	int32_t dataSize; 						// add later
	uint8_t *data;							// add later
	struct file *next;						// tip, struct missed
};
typedef struct file File;

/**
 * create struct folder und folder list.
 */

struct folder{

	FolderID folderID;
	char *folderName;
	File *subFile;
	struct folder *subFolder;
	int32_t itemCount;
	struct folder *next;
};
typedef struct folder Folder;

/**
 * create struct returnValue respectively for Folder and File,
 * which to store the necessary value
 */
struct returnfolder{	

	Folder *parentPosition; // change to Folder parentPosition
	Folder *destFolderPosition;
	Folder *preFolderPosition;
};
typedef struct returnfolder returnFolderValue;

struct returnfile{

	Folder *parentPosition;
	File *destFilePosition;   //File *previousFilePosition; currentFilePosition-- replace this member??? 
	File *preFilePosition;    // - does't work, with *next connect eachother rather than array 
};
typedef struct returnfile returnFileValue;

/**
 * Initializes the File System
 * @return the root FolderID
 */
FolderID fs_init();

/**
 * Destroys the File System
 */
void fs_destroy();

/**
 * Creates a new file in the given folder
 * @param name - name of the new file
 * @param parent - FolderID of the parent folder
 * @return the created file on success, negative value on error
 */
FileID fs_new_file(char *name, FolderID parent);

/**
 * Creates a new sub folder in the given folder
 * @param name - name of the new folder
 * @param parent - FolderID of the parent folder
 * @return the created folder on success, negative value on error
 */
FolderID fs_new_folder(char *name, FolderID parent);

/**
 * Deletes the given file
 * @param file - FileID of the file
 * @return 0 on success, -1 on error
 */
int32_t fs_delete_file(FileID file);

/**
 * Deletes the given folder
 * @param folder - FolderID of the folder
 * @return 0 on success, -1 on error
 */
int32_t fs_delete_folder(FolderID folder);

/**
 * Returns the id of the folder this file exists in
 * @param file - FileID of the file
 * @return The FolderID of the parent folder, negative value on error
 */
FolderID fs_get_file_parent(FileID file);

/**
 * Returns the file size in bytes
 * @param file - FileID of the file
 * @return The file size, negative value on error
 */
int32_t fs_get_file_size(FileID file);

/**
 * Returns the length of the file name in bytes
 * @param file - FileID of the file
 * @return Length of the file name in bytes, negative value on error
 */
int32_t fs_get_file_name_length(FileID file);

/**
 * Retrieves the file name of a given file. file_name must be a pointer to
 * buffer big enough for the file name. This function writes no more than
 * max_length bytes (including the null byte) of the file name into the buffer.
 * @param file - FileID of the file
 * @param file_name - Pointer to the buffer where the file name is to be written
 * @param max_length - Maximum number of bytes that are to be written to the buffer
 * @return 0 if the copying succeeded completely, any other value else
 */
int32_t fs_get_file_name(FileID file, char* file_name, uint8_t max_length);

/**
 * Get the parent folder of a given folder
 * @param folder - The id of the folder
 * @return The FolderID of the parent folder, negative value on error
 */
FolderID fs_get_folder_parent(FolderID folder);

/**
 * Returns the length of the folder name in bytes
 * @param folder - FolderID of the folder
 * @return Length of the folder name in bytes, negative value on error
 */
int32_t fs_get_folder_name_length(FolderID folder);

/**
 * Retrieves the folder name of a given folder. folder_name must be a pointer to
 * buffer big enough for the folder name. This function writes no more than
 * max_length bytes (including the null byte) of the folder name into the buffer.
 * @param folder - FolderID of the file
 * @param folder_name - Pointer to the buffer where the file name is to be written
 * @param max_length - Maximum number of bytes that are to be written to the buffer
 * @return 0 if the copying succeeded completely, any other value else
 */
int32_t fs_get_folder_name(FolderID folder, char* folder_name, uint32_t max_length);

/**
 * Returns the number of files in a folder
 * @param folder - FolderID of the folder
 * @return Number of files in the folder, negative value on error
 */
int32_t fs_get_folder_file_count(FolderID folder);

/**
 * Returns the number of folders in a folder
 * @param folder - FolderID of the folder
 * @return Number of folders in the folder, negative value on error
 */
int32_t fs_get_folder_folder_count(FolderID folder);

/**
 * Retrieves the files in a folder. files must be a pointer to a buffer big enough
 * for the FileIDs. This function writes no more than max_num_files FileIDs to
 * the buffer.
 * @param folder - FolderID of the folder
 * @param files - Pointer to the buffer where the FileIDs are to be written
 * @param max_num_files - Maximum number of FileIDs to write
 * @return 0 on success, a negative value otherwise
 */
int32_t fs_get_folder_files(FolderID folder, FileID* files, uint32_t max_num_files);

/**
 * Retrieves the folders in a folder. folders must be a pointer to a buffer big enough
 * for the FolderIDs. This function writes no more than max_num_folders FolderIDs to
 * the buffer.
 * @param folder - FolderID of the folder
 * @param folders - Pointer to the buffer where the FolderIDs are to be written
 * @param max_num_folders - Maximum number of FolderIDs to write
 * @return 0 on success, a negative value otherwise
 */
int32_t fs_get_folder_folders(FolderID folder, FolderID* folders, uint32_t max_num_folders);

/**
 * Writes data into a file
 * @param file - FileID of the file to write into
 * @param offset - the offset
 * @param length - length of data in bytes
 * @param data - bytes to write into the file
 * @return 0 on success, -1 on error
 */
int32_t fs_write_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data);

/**
 * Reads data from a file
 * @param file - FileID of the file to read from
 * @param offset - the offset
 * @param length - length of data in bytes
 * @param data - buffer to read data into
 * @return 0 on success, -1 on error
 */
int32_t fs_read_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data);

#endif /* FILESERVERAPI_H_ */
