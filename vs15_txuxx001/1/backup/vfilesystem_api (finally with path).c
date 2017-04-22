#include <vfilesystem_api.h>

/**
 * global variable for root folder
 */

Folder *rootPointer; 

/**
 * find the position for the parameter referred folder
 * @param rfv - store the get back value
 * @return the parentID of target folder, 0 on error
 */

FolderID folderFinder(FolderID id, Folder *folder, returnFolderValue* rfv){

	Folder* currentFolder = NULL;
	
	if(1 == id){						// in this case parent folder is root
		rfv-> ID = 1;
		rfv -> destFolderPosition = folder;
		rfv-> path = malloc(5*sizeof(char));
		strcpy(rfv-> path, "root");
		strcat(rfv -> path, "/");
		return rfv-> ID;
	}

	currentFolder = folder-> subFolder;

	if( NULL == currentFolder)				// subfolder in this folder is empty
		return rfv-> ID;

	if(id == currentFolder-> folderID){			// the first folder of subfolder is target folder
		strcpy(rfv-> path,folder-> folderName);  	// store the path e.g. folder/current/	
		strcat(rfv-> path, "/");
		strcat(rfv-> path, currentFolder-> folderName);
		strcat(rfv-> path, "/");				
		rfv-> destFolderPosition = currentFolder;				// store the position
		rfv-> preFolderPosition = currentFolder;    				// in this case pre and curr are same position
		rfv-> ID = currentFolder-> folderID;	
		rfv-> parentID = folder-> folderID;				
		return rfv-> ID;
	}

	currentFolder = currentFolder-> next;						// if first folder not the target, continue to run though
	Folder *preFolder = folder-> subFolder;
	
	while(currentFolder != NULL){

		if( id == currentFolder -> folderID ){				// if find the target
			strcpy(rfv-> path,folder-> folderName);  	// store the path e.g. folder/current/	
			strcat(rfv-> path, "/");
			strcat(rfv-> path, currentFolder-> folderName);
			strcat(rfv-> path, "/");				
			rfv-> destFolderPosition = currentFolder;				// store the position
			rfv-> preFolderPosition = preFolder;					// the previous pointer to be stored
			rfv-> ID = currentFolder -> folderID;
			rfv-> parentID = folder-> folderID;						// store the parentID

			return rfv-> ID;
		}

		folderFinder(id, currentFolder,rfv);				// recursion to find the target
		if( 0 != rfv-> ID){                               	// if find the target in subFolder e.g. "root/1/"
			char tmpPath[100]={0};
			strcpy(tmpPath,folder -> folderName);
			strcat(tmpPath,"/");    							// tipps, joint the path	
			strcat(tmpPath, rfv-> path);
			memset(rfv-> path, 0,sizeof(rfv-> path));		// tipps, 
			strcpy(rfv-> path, tmpPath);

			return rfv-> ID;					 
		}
		preFolder = preFolder-> next;
		currentFolder = currentFolder-> next;
	}
	return rfv-> ID;
}

FileID fileFinder(FileID id, Folder *folder, returnFileValue *rfv){

	File *currentFile = folder-> subFile;

	if(NULL == currentFile)							// subFile is empty
		return rfv -> ID;

	if(id == currentFile -> fileID){				// first file is target
		strcpy(rfv-> path , folder -> folderName);	
		strcat(rfv -> path, "/");
		rfv -> destFilePosition = currentFile;	
		rfv -> preFilePosition = currentFile;	
		rfv -> ID = currentFile -> fileID;	
		rfv -> parentID = folder -> folderID;		

		return rfv -> ID;
	}

	currentFile = currentFile -> next;
	File *preFile = folder -> subFile;

	while( NULL != currentFile && id != currentFile -> fileID ){
		currentFile = currentFile -> next; 
		preFile = preFile -> next;
	}

	if(NULL != currentFile){						// target file found in the list of this folder
		strcpy(rfv-> path , folder -> folderName);	// set the path e.g. "1/" into rfv
		strcat(rfv -> path, "/");
		rfv -> destFilePosition = currentFile;		// set the file position into rfv
		rfv -> preFilePosition = preFile;			// set the previous file position
		rfv -> ID = currentFile -> fileID;			// set the file id into rfv
		rfv -> parentID = folder -> folderID; 		// set the parent folderID
		return rfv -> ID;
	}
	else{
		Folder *currentFolder = folder -> subFolder;	// target file not in this folder, 
														// continue to find in the subfolder
		while(NULL != currentFolder){

			fileFinder(id, currentFolder, rfv);

			if(0 != rfv-> ID){								// if target file in the subfolder 
				strcat("/",rfv -> path);					// joint the path e.g. "root/1/"
				strcat(rfv-> path, folder -> folderName);
				return rfv-> ID;
			}

			currentFolder = currentFolder -> next; 				// as not found,continue to run though this subfolder
		}
	}
	return rfv -> ID;
}

FolderID fs_init() {

		rootPointer = malloc(sizeof(Folder));
		rootPointer-> folderID = 1;
		rootPointer-> folderName = "root";
		rootPointer-> subFile = NULL;
		rootPointer-> subFolder = NULL;
		rootPointer-> itemCount = 0;
		rootPointer-> next = NULL;
		
		return rootPointer-> folderID;
	}
	return INVALID_HANDLE;
}

void fs_destroy() {


	memset(rootPointer, 0, sizeof(Folder));      	// free the root node in the tree

}

FileID fs_new_file(char *name, FolderID parent){
	
	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0
	folderFinder(parent, rootPointer, get);

	File *newFile = (File *)malloc(sizeof(File)); 		
								     								// tipps, pointer is better than struct				
		rootPointer -> itemCount++;									// hard to make sure the name of variable
		newFile-> fileID = rootPointer -> itemCount+1;	
		newFile-> fileName = malloc(strlen(name)*sizeof(char));
		strcpy(newFile -> fileName, name);
		newFile -> next = NULL;

		File *helper = get-> destFolderPosition -> subFile;
		if(NULL == helper)	
			get-> destFolderPosition -> subFile = newFile;	// attention! 

		else{
			while(NULL != helper->next )
				helper = helper -> next;

			helper-> next = newFile;					//attention! helper = helper -> next, helper = newfile, doesn't work

		}
	free(get-> path);
	free(get);

	return newFile-> fileID;
}

FolderID fs_new_folder(char *name, FolderID parent){
	
	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0
	folderFinder(parent, rootPointer, get);
	
	Folder *newFolder = (Folder *)malloc(sizeof(Folder)); 
			     
		rootPointer ->itemCount++;								
		newFolder-> folderID = rootPointer ->itemCount+1;	
		newFolder-> folderName = malloc(strlen(name)*sizeof(char));
		strcpy(newFolder -> folderName,name);
		newFolder-> next = NULL;
		newFolder-> subFile = NULL;
		newFolder-> subFolder = NULL; 

		Folder *helper = get-> destFolderPosition -> subFolder;
		if(NULL == helper)	
			get-> destFolderPosition -> subFolder = newFolder;	

		else{
			while(NULL != helper->next )
				helper = helper -> next;

			helper-> next = newFolder;					

		}  
	
	free(get->path);
	free(get);

	return newFolder-> folderID;
}
	

int32_t fs_delete_file(FileID file) {

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0												
	fileFinder(file, rootPointer, get);

	file *helper = get -> destFilePosition;
	if( NULL != helper){													
		if( get -> destFilePosition == get -> preFilePosition){				// as target file is the first file in subfile
			get -> destFilePosition = get -> destFilePosition -> next;
			free(helper -> data);
			free(helper -> fileName);
			free(helper);	
			free(get->path);
			free(get);

			return 0;	
		}

	else{
		get -> preFilePosition -> next = get -> destFilePosition -> next;
		free(get -> destFilePosition -> data);
		free(get -> destFilePosition -> fileName);
		free(get -> destFilePosition);
		free(get->path);
		free(get);

		return 0;
		}
	}	
	else
		return INVALID_HANDLE;
}

int32_t fs_delete_folder(FolderID folder) {

	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																
	folderFinder(parent, rootPointer, get);


	File *fileHelper = get -> destFolderPosition ->  subFile;
	while(NULL != fileHelper){
		File *tmp = fileHelper -> next;
		fs_delete_file(fileHelper -> fileID);	
		fileHelper = tmp;					
	}
																

	return 0;
}

FolderID fs_get_file_parent(FileID file) {

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0												
	fileFinder(file, rootPointer, get);

	if(0 != get -> parentID){
		FolderID rslt = get -> parentID;
		free(get-> path);
		free(get);
		return rslt;
	}
	else 
		return INVALID_HANDLE;
}

int32_t fs_get_file_size(FileID file) {

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0												
	fileFinder(file, rootPointer, get);

	int32_t fileSize = 0;
	if(NULL != get -> destFilePosition){
		file *helper = get -> destFilePosition;
		while(????){
			fileSize++;
			helper++;
		}
	}
	else 
		return INVALID_HANDLE;
}

int32_t fs_get_file_name_length(FileID file) {

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0										
	fileFinder(file, rootPointer, get);

	if(NULL != get -> destFilePosition -> fileName){
		int32_t length = strlen(get -> destFilePosition -> fileName);
		free(get-> path);
		free(get);
		return length;
	}

	else 
		return INVALID_HANDLE;
}

int32_t fs_get_file_name(FileID file, char* file_name, uint8_t max_length) {

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0											
	fileFinder(file, rootPointer, get);

	char *newName = malloc(max_length*sizeof(char));						// must store in memery or disk 
	strcpy(newName, file_name);		

	if(0 != get -> ID){
		free(get -> destFilePosition -> fileName);
		get -> destFilePosition -> fileName = newName;
		free(get-> path);
		free(get);
			return 0;		
	}

	else 
		return INVALID_HANDLE;
}

FolderID fs_get_folder_parent(FolderID folder) {

	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0
	folderFinder(folder, rootPointer, get);

	if(0 != get -> parentID){
		FolderID rslt = get -> parentID;
		free(get-> path);
		free(get);

		return rslt;
	}

	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_name_length(FolderID folder) {

	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0	
	folderFinder(folder, rootPointer, get);

	if(NULL != get -> destFolderPosition -> folderName){
		int32_t length = strlen(get -> destFolderPosition -> folderName);
		free(get-> path);
		free(get);
		return length;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_name(FolderID folder, char* folder_name, uint32_t max_length) {
	
	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0
	folderFinder(folder, rootPointer, get);

	char *newName = malloc(max_length* sizeof(char));
	strcpy(newName, folder_name);

	if(NULL != get -> destFolderPosition -> folderName){
		free(get -> destFolderPosition -> folderName);
		get -> destFolderPosition -> folderName = newName;
		free(get-> path);
		free(get);
		return 0;
	}

	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_file_count(FolderID folder) {

	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0
	folderFinder(folder, rootPointer, get);

	if(NULL != get -> destFolderPosition ){
		int32_t count =0;
		File *helper = get -> destFolderPosition -> subFile;
		while(NULL != helper){
			count++;
			helper = helper -> next;
		}

		return count;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_folder_count(FolderID folder) {

	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0
	folderFinder(folder, rootPointer, get);

	if(NULL != get -> destFolderPosition ){
		int32_t count =0;
		Folder *helper = get -> destFolderPosition -> subFolder;
		while(NULL != helper){
			count++;
			helper = helper -> next;
		}

		return 0;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_files(FolderID folder, FileID* files, uint32_t max_num_files) {
	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																
	folderFinder(folder, rootPointer, get);

	FileID *retrieve = malloc(max_num_files*sizeof(FileID));
	int32_t count = 0;
	if(NULL != get -> destFolderPosition){		
		File *helper = get -> destFolderPosition -> subFile;
		while(NULL != helper && count < max_num_files){
			retrieve[count] = helper -> fileID;
			helper =  helper -> next;
			count++;
		}
		files = retrieve;
		return 0;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_folders(FolderID folder, FolderID* folders, uint32_t max_num_folders) {
	returnFolderValue *get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																
	folderFinder(folder, rootPointer, get);

	FolderID *retrieve = malloc(max_num_folders*sizeof(FolderID));
	int32_t count = 0;
	if(NULL != get -> destFolderPosition){		
		Folder *helper = get -> destFolderPosition -> subfolder;
		while(NULL != helper && count < max_num_files){
			retrieve[count] = helper -> folderID;
			helper =  helper -> next;
			count++;
		}
		folders = retrieve;
		return 0;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_write_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data){

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																										
	fileFinder(file, rootPointer, get);

	uint8_t *newData = malloc((length + fileSize - offset) * sizeof(uint8_t));
	uint32_t count = 0;
	if( NULL != get -> destFilePosition){
		uint8_t *helper = get -> destFilePosition -> data;
		while(count < offset){
			newData[count] = helper[count];
			count++;
		}
		while(count-offset+1 < length){
			newData[count] = data[count-offset+1];
			count++;
		}
		free(helper);
		get -> destFilePosition -> data = newData;
		get -> destFilePosition -> dataLen = length + fileSize - offset;
		return 0;	
	}
	else
		return -1;

}

int32_t fs_read_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data){

	returnFileValue *get = (returnFileValue *) malloc(sizeof(returnFileValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																										
	fileFinder(file, rootPointer, get);

	uint8_t *tmpData = malloc(length * sizeof(uint8_t));
	uint32_t count = 0;
	if( NULL != get -> destFilePosition){
		uint8_t *helper = get -> destFilePosition -> data;
		while(NULL != helper && count < length ){
			tmpData[count] = helper[offset+count];
			count++;
		}
		data = tmpData;
		return 0;
	}
	else
		return -1;
}
