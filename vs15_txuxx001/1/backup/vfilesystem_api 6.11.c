#include <vfilesystem_api.h>

/**
 * global variable for root folder 
 */

Folder *rootPointer; 
returnFolderValue *rfdv;
returnFileValue *rflv;

/**
 * find the position for the parameter referred folder
 * @param id, folderID
 * @param folder, just transfer the root pointer
 * @param rfdv - store the get back value for folder
 * @return 1 on success, 0 on error
 */

uint32_t folderFinder(FolderID id, Folder *folder, returnFolderValue* rfdv){

	Folder* currentFolder = NULL;
	
	if(1 == id){											// in this case parent folder is root		
		rfdv -> destFolderPosition = folder;
		
		return 1;
	}

	currentFolder = folder-> subFolder;

	if( NULL == currentFolder)				// subfolder in this folder is empty
		return 0;

	if(id == currentFolder-> folderID){							// the first folder of subfolder is target folder
		rfdv-> destFolderPosition = currentFolder;				// store the position
		rfdv-> preFolderPosition = currentFolder;    			// in this case pre and curr are same position
		rfdv -> parentPosition = folder;							// set the parent position
		return 1;
	}

	currentFolder = currentFolder-> next;						// if first folder not the target, continue to run though
	Folder *preFolder = folder-> subFolder;
	
	while(currentFolder != NULL){
		if( id == currentFolder -> folderID ){				// if find the target			
			rfdv-> destFolderPosition = currentFolder;				// store the position
			rfdv-> preFolderPosition = preFolder;					// the previous pointer to be stored
			rfdv -> parentPosition = folder;
			return 1;
		}
																	// recursion to find the target
		if( folderFinder(id, currentFolder,rfdv))                 
			return 1;					 
		
		preFolder = preFolder-> next;
		currentFolder = currentFolder-> next;
	}
	return 0;
}

/**
 * find the position for the parameter referred folder
 * @param id, fileID
 * @param folder, just transfer the root pointer
 * @param rflv - store the get back value for file
 * @return 1 on success, 0 on error
 */

uint32_t fileFinder(FileID id, Folder *folder, returnFileValue *rflv){

	File *currentFile = folder-> subFile;

	if(NULL == currentFile)							// subFile is empty
		return 0;

	if(id == currentFile -> fileID){				// first file is target
		
		rflv -> destFilePosition = currentFile;	
		rflv -> preFilePosition = currentFile;	
		rflv -> parentPosition = folder;	

		return 1;
	}

	currentFile = currentFile -> next;
	File *preFile = folder -> subFile;

	while( NULL != currentFile && id != currentFile -> fileID ){
		currentFile = currentFile -> next; 
		preFile = preFile -> next;
	}

	if(NULL != currentFile){						// found target file  
		
		rflv -> destFilePosition = currentFile;		// set the file position into rflv
		rflv -> preFilePosition = preFile;			// set the previous file position
		rflv -> parentPosition = folder;
		return 1;
	}
	else{
		Folder *currentFolder = folder -> subFolder;	// target file not in this folder, 
														// continue to find in the subfolder
		while(NULL != currentFolder){			

			if(fileFinder(id, currentFolder, rflv))		// if target file in the subfolder 
				return 1;		

			currentFolder = currentFolder -> next; 				// as not found,continue to run though this subfolder
		}
	}
	return 0;
}

FolderID fs_init() {

		rootPointer = malloc(sizeof(Folder));
		rootPointer-> folderID = 1;
		rootPointer-> folderName = "root";
		rootPointer-> subFile = NULL;
		rootPointer-> subFolder = NULL;
		rootPointer-> itemCount = 0;
		rootPointer-> next = NULL;

		rflv = malloc(sizeof(returnFileValue));			// initialize global variable rflv
		rfdv = malloc(sizeof(returnFolderValue));		// initialize global variable rfdv
		
		return rootPointer-> folderID;
	}

void resetRflv(){
	rflv -> destFilePosition = NULL;
	rflv -> preFilePosition = NULL;
	rflv -> parentPosition = NULL;
}

void resetRfdv(){
	rfdv -> destFolderPosition = NULL;
	rfdv -> preFolderPosition = NULL;
	rfdv -> parentPosition = NULL;
}

void fs_destroy() {


	memset(rootPointer, 0, sizeof(Folder));      	// free the root node in the tree

}

FileID fs_new_file(char *name, FolderID parent){

	resetRfdv();	

	if(folderFinder(parent, rootPointer, rfdv)){

		File *newFile = (File *)malloc(sizeof(File)); 		
								     								// tipps, pointer is better than struct				
			rootPointer -> itemCount++;									// hard to make sure the name of variable
			newFile-> fileID = rootPointer -> itemCount+1;	
			newFile-> fileName = malloc(strlen(name)*sizeof(char));
			strcpy(newFile -> fileName, name);
			newFile -> next = NULL;
			rfdv -> destFolderPosition -> itemCount++;				// itemCount not initialize ???????

			File *helper = rfdv-> destFolderPosition -> subFile;
			if(NULL == helper)	
				rfdv-> destFolderPosition -> subFile = newFile;	// attention! 

			else{
				while(NULL != helper->next )
					helper = helper -> next;

				helper-> next = newFile;			//attention! helper = helper -> next, helper = newfile, doesn't work

			}	
		return newFile-> fileID;
	}
	return INVALID_HANDLE;
}

FolderID fs_new_folder(char *name, FolderID parent){

	resetRfdv();
														
	if(folderFinder(parent, rootPointer, rfdv)){
	
		Folder *newFolder = (Folder *)malloc(sizeof(Folder)); 
			     
			rootPointer ->itemCount++;								
			newFolder-> folderID = rootPointer ->itemCount+1;	
			newFolder-> folderName = malloc(strlen(name)*sizeof(char));
			strcpy(newFolder -> folderName,name);
			newFolder-> next = NULL;
			newFolder-> subFile = NULL;
			newFolder-> subFolder = NULL; 
			rfdv -> destFolderPosition -> itemCount++;

			Folder *helper = rfdv-> destFolderPosition -> subFolder;
			if(NULL == helper)	
				rfdv-> destFolderPosition -> subFolder = newFolder;	

			else{
				while(NULL != helper->next )
					helper = helper -> next;

				helper-> next = newFolder;	
			}  
		return newFolder-> folderID;
	}
	return INVALID_HANDLE;
}	

int32_t fs_delete_file(FileID file) {

	resetRflv();
	
	if( fileFinder(file, rootPointer, rflv)){		
		File *helper = rflv -> destFilePosition;											
		if( rflv -> destFilePosition == rflv -> preFilePosition){				// as target file is the first file in subfile
			rflv -> parentPosition ->subFile = rflv -> destFilePosition -> next;
			free(helper -> data);
			free(helper -> fileName);
			free(helper);			

			return 0;	
		}

	else{
		rflv -> preFilePosition -> next = rflv -> destFilePosition -> next;
		free(rflv -> destFilePosition -> data);
		free(rflv -> destFilePosition -> fileName);
		free(rflv -> destFilePosition);

		return 0;
		}
		
	}	
	return INVALID_HANDLE;
}

int32_t fs_delete_folder(FolderID folder) {
	
	if(folderFinder(folder, rootPointer, rfdv)){
		File *fileHelper = rfdv -> destFolderPosition ->  subFile;
		File *tmp = NULL;
		while(NULL != fileHelper){									// delete all of files in subfile
			tmp = fileHelper -> next;
			fs_delete_file(fileHelper -> fileID);	
			fileHelper = tmp;		
		}			
		Folder *folderHelper = rfdv -> destFolderPosition -> subFolder; 
	}	
	return 0;
}

FolderID fs_get_file_parent(FileID file) {		

	resetRflv();									
	
	if(fileFinder(file, rootPointer, rflv)){

		FolderID rslt = rflv -> parentPosition -> folderID;
		
		return rslt;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_file_size(FileID file) {

	resetRflv();

	if(fileFinder(file, rootPointer, rflv))
		return rflv -> destFilePosition -> dataSize;

	return INVALID_HANDLE;
}

int32_t fs_get_file_name_length(FileID file) {

	resetRflv();

	if(fileFinder(file, rootPointer, rflv)){
		int32_t length = strlen(rflv -> destFilePosition -> fileName);
		
		return length;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_file_name(FileID file, char* file_name, uint8_t max_length) {

	resetRflv();

	char *newName = malloc(max_length*sizeof(char));						// must store in memery or disk 
	strcpy(newName, file_name);		

	if(fileFinder(file, rootPointer, rflv)){
		free(rflv -> destFilePosition -> fileName);
		rflv -> destFilePosition -> fileName = newName;
		
		return 0;		
	}
	return INVALID_HANDLE;
}

FolderID fs_get_folder_parent(FolderID folder) {		

	resetRfdv();

	if(folderFinder(folder, rootPointer, rfdv)){
		FolderID rslt = rfdv -> parentPosition -> folderID;
		
		return rslt;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_folder_name_length(FolderID folder) {	

	resetRfdv();

	if(folderFinder(folder, rootPointer, rfdv)){
		int32_t length = strlen(rfdv -> destFolderPosition -> folderName);
		
		return length;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_folder_name(FolderID folder, char* folder_name, uint32_t max_length) {	

	resetRfdv();
	char *newName = malloc(max_length* sizeof(char));
	strcpy(newName, folder_name);

	if(folderFinder(folder, rootPointer, rfdv)){
		free(rfdv -> destFolderPosition -> folderName);
		rfdv -> destFolderPosition -> folderName = newName;
		
		return 0;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_folder_file_count(FolderID folder) {	

	resetRfdv();
	if(folderFinder(folder, rootPointer, rfdv)){
		int32_t count =0;
		File *helper = rfdv -> destFolderPosition -> subFile;
		while(NULL != helper){
			count++;
			helper = helper -> next;
		}
		return count;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_folder_folder_count(FolderID folder) {

	resetRfdv();
	if(folderFinder(folder, rootPointer, rfdv)){
		int32_t count =0;
		Folder *helper = rfdv -> destFolderPosition -> subFolder;
		while(NULL != helper){
			count++;
			helper = helper -> next;
		}
		return 0;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_folder_files(FolderID folder, FileID* files, uint32_t max_num_files) {
	
	resetRfdv();	
	int32_t count = 0;
	if(folderFinder(folder, rootPointer, rfdv)){		
		FileID *retrieve = malloc(max_num_files*sizeof(FileID));
		File *helper = rfdv -> destFolderPosition -> subFile;
		while(NULL != helper && count < max_num_files){
			retrieve[count] = helper -> fileID;
			helper =  helper -> next;
			count++;
		}
		files = retrieve;
		return 0;
	}
	return INVALID_HANDLE;
}

int32_t fs_get_folder_folders(FolderID folder, FolderID* folders, uint32_t max_num_folders) {
	
	resetRfdv();		
	int32_t count = 0;
	if(folderFinder(folder, rootPointer, rfdv)){		
		FolderID *retrieve = malloc(max_num_folders*sizeof(FolderID));
		Folder *helper = rfdv -> destFolderPosition -> subFolder;
		while(NULL != helper && count < max_num_folders){
			retrieve[count] = helper -> folderID;
			helper =  helper -> next;
			count++;
		}
		folders = retrieve;

		return 0;
	}
	return INVALID_HANDLE;
}

int32_t fs_write_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data){
	
	resetRflv();
	uint32_t count = 0;
	if( fileFinder(file, rootPointer, rflv)){
		uint8_t *newData = malloc((length + rflv -> destFilePosition -> dataSize - offset) * sizeof(uint8_t));
		uint8_t *helper = rflv -> destFilePosition -> data;
		while(count < offset){
			newData[count] = helper[count];
			count++;
		}
		while(count-offset+1 < length){
			newData[count] = data[count-offset+1];
			count++;
		}
		free(helper);
		rflv -> destFilePosition -> data = newData;
		rflv -> destFilePosition -> dataSize = length + rflv -> destFilePosition -> dataSize - offset;
		return 0;	
	}
	return -1;
}

int32_t fs_read_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data){
	
	resetRflv();																						
	uint32_t count = 0;
	if( fileFinder(file, rootPointer, rflv)){
		uint8_t *tmpData = malloc(length * sizeof(uint8_t));
		uint8_t *helper = rflv -> destFilePosition -> data;
		while(NULL != helper && count < length ){
			tmpData[count] = helper[offset+count];
			count++;
		}
		data = tmpData;

		return 0;
	}
	return -1;
}
 