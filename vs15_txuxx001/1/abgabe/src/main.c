#include <vfilesystem_api.h>

Folder *rootPointer; 
returnFolderValue *rfdv;
returnFileValue *rflv;

int main(int argc, char **args) {

	FolderID root = fs_init();
	
	FolderID fd2 = fs_new_folder("folder1", root);	

	FileID fl3 = fs_new_file("file3",fd2);	
	FileID fl4 = fs_new_file("file4",fd2);	
	FileID fl5 = fs_new_file("file5",fd2);	
	FolderID fd6= fs_new_folder("folder6",fd2);
	FolderID fd7= fs_new_folder("folder7",fd2);
	FolderID fd8= fs_new_folder("folder8",fd2);

	FolderID fd9 = fs_new_folder("folder9", root);
	FolderID fd10 = fs_new_folder("folder10", root);
	

	FileID fl11 = fs_new_file("file11",root);	
	FileID fl12 = fs_new_file("file12",root);	
	FileID fl13 = fs_new_file("file13",root);
	FileID fl14 = fs_new_file("file14",fd6);

	// write and read
	uint32_t offset =10;
	uint32_t length = 8;
	uint8_t *data ="12345678\0";
	fs_write_file(fl11, offset, length, data);
	printf("context : %s datasize :%d\n", rflv -> destFilePosition -> data,rflv -> destFilePosition -> dataSize);

	uint32_t Roffset =5;
	uint32_t Rlength = 3;
	uint8_t *Rdata = malloc((Rlength+1) * sizeof(uint8_t));      

	fs_read_file(fl11, Roffset, Rlength, Rdata);
	printf("context : %s\n", Rdata);
	
	// get folders and files
	uint32_t fileCount = fs_get_folder_file_count(root);
	FileID *files = malloc(fileCount*sizeof(FileID));            
	uint32_t rslt = fs_get_folder_files(fd2, files, fileCount);
	printf("%d\n", rslt);
	int i;
	for(i=0;i<fileCount;i++)
		printf("file id: %d\n",files[i] );

	uint32_t folderCount = fs_get_folder_folder_count(fd2);
	FileID *folders = malloc(folderCount*sizeof(FolderID)); 
	fs_get_folder_folders(fd2, folders, folderCount);
	for(i=0;i<folderCount;i++)
		printf("folder id: %d\n",folders[i] );
	
	//  4 methods about file
	printf("%d\n", fs_get_file_parent(fl4));
	printf("%d\n",fs_get_file_size(fl5));
	printf("%d\n",fs_get_file_name_length(fl3) );
	printf("%d\n",fs_get_file_name(fl4, "newfile4", 8) );
	fileFinder(fl4,rootPointer,rflv);
	printf("%s\n", rflv -> destFilePosition -> fileName);
	
	//  5 methods about folder
	printf("%d\n",fs_get_folder_parent(fd8) );
	printf("%d\n",fs_get_folder_name_length(fd6) );
	printf("%d\n",fs_get_folder_name(fd7, "newfolder7", 10) );
	folderFinder(fd7,rootPointer,rfdv);
	printf("%s\n", rfdv -> destFolderPosition -> folderName);
	printf("%d\n",fs_get_folder_file_count(fd2));
	printf("%d\n",fs_get_folder_folder_count(fd2));	

	// check new_file
	fileFinder(fl4,rootPointer,rflv);
	printf("preid %d currid %d afterid %d\n",rflv -> preFilePosition -> fileID, rflv -> destFilePosition -> fileID,
	rflv -> destFilePosition -> next -> fileID );	
	
	// check the new_folder()	
	folderFinder(fd7,rootPointer,rfdv);
	printf("preid %d currid %d \n",rfdv -> preFolderPosition -> folderID, rfdv -> destFolderPosition -> folderID );
	

	// check fs_delete_file() and if the filelist right rebuild
	fs_delete_file(fl4);
	fileFinder(fl5,rootPointer,rflv);
	printf("preid %d currid %d\n",rflv -> preFilePosition -> fileID, rflv -> destFilePosition -> fileID );
		
	// check fs_delete_folder() and if the folderlist right rebuild
	fs_delete_folder(fd6);
	folderFinder(fd7,rootPointer,rfdv);
	printf("preid %d currid %d\n",rfdv -> preFolderPosition -> folderID, rfdv -> destFolderPosition -> folderID );
	printf("------\n");

	fs_destroy();	

	return 0;
}
