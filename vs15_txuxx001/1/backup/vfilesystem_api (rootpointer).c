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

void finderInit(returnFolderValue *get){

	get = (returnFolderValue *) malloc(sizeof(returnFolderValue));
	get-> path = malloc(100*sizeof(char));
	get-> ID = 0;																// initialize get with id =0												// attention, root must not be null

}

FolderID fs_init() {

	int status = mkdir("root",0777);

	if(0 == status){

		root-> folderID = 1;
		root-> folderName = "root";
		root-> subFile = NULL;
		root-> subFolder = NULL;
		root-> itemCount = 0;
		root-> next = NULL;
		printf("create root directory success\n");

		return root-> folderID;
	}
	return INVALID_HANDLE;
}

void fs_destroy() {
	

	int status = rmdir("root");
	printf("status %d\n", status);
	if(0 == status)
		printf(" delete root success\n");

	memset(&root, 0, sizeof(Folder));      	// free the root node in the tree

}

FileID fs_new_file(char *name, FolderID parent){
	//if(access("parentName"))
	returnFolderValue *get;
	finderInit(get);
	folderFinder(parent, rootPointer, get);

	char *newFilePath = malloc((strlen(name)+strlen(get->path)+2)*sizeof(char));
	strcpy(newFilePath, get-> path);
	strcat(newFilePath, name);

	printf("%s\n",newFilePath );
	FILE *fp;
	fp = fopen(newFilePath,"w");
	File *newFile = (File *)malloc(sizeof(File)); 
	if(NULL != fp){	
		printf("fp not null\n");	     								// tipps, pointer is better than struct				
		root.itemCount++;									// hard to make sure the name of variable
		newFile-> fileID = root.itemCount+1;	
		newFile-> fileName = name;
		newFile -> next = NULL;
		File *helper = get-> destFolderPosition -> subFile;

		if(NULL == helper)	
			get-> destFolderPosition -> subFile = newFile;	// attention! 

		else{
			while(NULL != helper->next )
				helper = helper -> next;

			helper-> next = newFile;					//attention! helper = helper -> next, helper = newfile, doesn't work

		}
		printf("created new file %s success \n", name);	   
	}
	else 
		printf("created new file %s failure\n",name);

	free(get-> path);
	free(get);
	free(newFilePath);

	return newFile-> fileID;
}

FolderID fs_new_folder(char *name, FolderID parent){
	
	returnFolderValue *get;
	finderInit(get);
	folderFinder(parent, rootPointer, get);
	
	char *newFolderPath = malloc((strlen(name)+strlen(get->path)+1)*sizeof(char));	
	strcpy(newFolderPath, get-> path);	
	strcat(newFolderPath, name);
	printf("%s\n", newFolderPath);
	int status = mkdir(newFolderPath,0777);
	
	Folder *newFolder = (Folder *)malloc(sizeof(Folder)); 
	if(0 == status){		     
		root.itemCount++;								
		newFolder-> folderID = root.itemCount+1;	
		newFolder-> folderName = name;
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
		printf("created new folder %s success \n", name);	   
	}
	free(get->path);
	free(get);
	free(newFolderPath);

	return newFolder-> folderID;
}

int32_t fs_delete_file(FileID file) {
	return 0;
}

int32_t fs_delete_folder(FolderID folder) {
	return 0;
}

FolderID fs_get_file_parent(FileID file) {

	returnFolderValue *get;
	finderInit(get);												
	fileFinder(file, rootPointer, get);

	if(0 != get -> parentID)
		return get -> parentID;

	else 
		return INVALID_HANDLE;
}

int32_t fs_get_file_size(FileID file) {


}

int32_t fs_get_file_name_length(FileID file) {

	returnFolderValue *get;
	finderInit(get);											
	fileFinder(file, rootPointer, get);

	if(NULL != get -> destFilePosition -> fileName){
		int32_t length = strlen(get -> destFilePosition -> fileName);

		return length;
	}

	else 
		return INVALID_HANDLE;
}

int32_t fs_get_file_name(FileID file, char* file_name, uint8_t max_length) {

	returnFolderValue *get;
	finderInit(get);												
	fileFinder(file, rootPointer, get);

	char *newName = malloc(max_length*sizeof(char));						// must store in memery or disk 
	strcpy(newName, file_name);		

	if(0 != get -> ID){
		free(get -> destFilePosition -> fileName);
		get -> destFilePosition -> fileName = newName;
		
			return 0;
		
	}

	else 
		return INVALID_HANDLE;
}

FolderID fs_get_folder_parent(FolderID folder) {

	returnFolderValue *get;
	finderInit(get);
	folderFinder(folder, rootPointer, get);

	if(0 != get -> parentID)
		return get -> parentID;

	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_name_length(FolderID folder) {

	returnFolderValue *get;
	finderInit(get);	
	folderFinder(folder, rootPointer, get);

	if(NULL != get -> destFolderPosition -> folderName){
		int32_t length = strlen(get -> destFolderPosition -> folderName);
		return length;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_name(FolderID folder, char* folder_name, uint32_t max_length) {
	
	returnFolderValue *get;
	finderInit(get);
	folderFinder(folder, rootPointer, get);

	char *newName = malloc(max_length* sizeof(char));
	strcpy(newName, folder_name);

	if(NULL != get -> destFolderPosition -> folderName){
		free(get -> destFolderPosition -> folderName);
		get -> destFolderPosition -> folderName = newName;

		return 0;
	}

	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_file_count(FolderID folder) {

	returnFolderValue *get;
	finderInit(get);
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

	returnFolderValue *get;
	finderInit(get);
	folderFinder(folder, rootPointer, get);

	if(NULL != get -> destFolderPosition ){
		int32_t count =0;
		Folder *helper = get -> destFolderPosition -> subFolder;
		while(NULL != helper){
			count++;
			helper = helper -> next;
		}

		return count;
	}
	else
		return INVALID_HANDLE;
}

int32_t fs_get_folder_files(FolderID folder, FileID* files, uint32_t max_num_files) {
	return 0;
}

int32_t fs_get_folder_folders(FolderID folder, FolderID* folders, uint32_t max_num_folders) {
	return 0;
}

int32_t fs_write_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data){


}

int32_t fs_read_file(FileID file, uint32_t offset, uint32_t length, uint8_t *data){

}
