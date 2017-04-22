package de.hsrm.cs.wwwvs.filesystem;

import java.io.IOException;
import java.util.List;

public interface Filesystem {
	
	/**
	 * Returns the ID of the root folder of the filesystem 
	 * (should be static 0)
	 * @return 0
	 * @throws IOException - if some error occurs
	 */
	public int get_root_folder() throws IOException;

	/**
	 * Creates a new file in the given folder
	 * 
	 * @param name
	 *            - name of the new file
	 * @param parent
	 *            - ID of the parent folder
	 * @return the created file on success
	 * @throws NullPointerException - If name is null
	 * @throws IOException - if some error occurs
	 */
	public int new_file(String name, int parent) throws IOException;

	/**
	 * Creates a new sub folder in the given folder
	 * 
	 * @param name
	 *            - name of the new folder
	 * @param parent
	 *            - ID of the parent folder
	 * @return the created folder on success
	 * @throws NullPointerException - If name is null
	 * @throws IOException - if some error occurs
	 */
	public int new_folder(String name, int parent) throws IOException;
	

	/**
	 * Deletes the given file
	 * 
	 * @param file
	 *            - ID of the file
	 * @throws IOException - if some error occurs
	 */
	public void delete_file(int file) throws IOException;

	/**
	 * Deletes the given folder
	 * 
	 * @param folder
	 *            - ID of the folder
	 * @throws IOException - if some error occurs
	 */
	public void delete_folder(int folder) throws IOException;

	/**
	 * Returns the id of the folder this file exists in
	 * 
	 * @param file
	 *            - ID of the file
	 * @return The ID of the parent folder
	 * @throws IOException - if some error occurs
	 */
	public int get_file_parent(int file) throws IOException;

	/**
	 * Returns the file size in bytes
	 * 
	 * @param file
	 *            - ID of the file
	 * @return The file size
	 * @throws IOException - if some error occurs
	 */
	public int get_file_size(int file) throws IOException;

	/**
	 * Retrieves the file name of a given file.
	 * 
	 * @param file
	 *            - ID of the file
	 * @return the name of the file
	 * @throws IOException - if some error occurs
	 */
	String get_file_name(int file) throws IOException;

	/**
	 * Get the parent folder of a given folder
	 * 
	 * @param folder
	 *            - The ID of the folder
	 * @return The ID of the parent folder
	 * @throws IOException - if some error occurs
	 */
	int get_folder_parent(int folder) throws IOException;

	/**
	 * Retrieves the folder name of a given folder.
	 * 
	 * @param folder
	 *            - ID of the file
	 * @return the name of the folder
	 * @throws IOException - if some error occurs
	 */
	String get_folder_name(int folder) throws IOException;

	/**
	 * Returns the number of files in a folder
	 * 
	 * @param folder
	 *            - ID of the folder
	 * @return Number of files in the folder
	 * @throws IOException - if some error occurs
	 */
	int get_folder_file_count(int folder) throws IOException;

	/**
	 * Returns the number of folders in a folder
	 * 
	 * @param folder
	 *            - ID of the folder
	 * @return Number of folders in the folder
	 * @throws IOException - if some error occurs
	 */
	int get_folder_folder_count(int folder) throws IOException;

	/**
	 * Retrieves the files in a folder.
	 * 
	 * @param folder
	 *            - FolderID of the folder
	 * @return List of the file IDs of the given folder
	 * @throws IOException - if some error occurs
	 */
	List<Integer> get_folder_files(int folder) throws IOException;

	/**
	 * Retrieves the folders in a folder. 
	 *  
	 * @param folder
	 *            - FolderID of the folder
	 * @return List of the folder IDs of the given folder
	 * @throws IOException - if some error occurs
	 */
	List<Integer> get_folder_folders(int folder) throws IOException;

	/**
	 * Writes bytes to the file. The file will not be deleted before writing
	 * into it. This method just writes into the file at the specified position
	 * regardless of previous contents. Gaps are bridged with zeros. If the
	 * offset is negative, no bytes are written.
	 * 
	 * @param offset
	 *            Position to start writing to
	 * @param data
	 *            Byte array to write into file
	 * @throws IOException - if some error occurs
	 */
	void write_file(int file, int offset, byte[] data) throws IOException;

	/**
	 * Reads <code>length</code> bytes from the file, starting at <code>
	 * offset</code> and returns them as a byte array. If the offset is equal or
	 * less 0 or <code>offset + length</code> is greater than the length of the
	 * file a byte array of size 0 is returned.
	 * 
	 * @param file - ID of file to read
	 * @param offset -
	 *            Position to start reading from
	 * @param length
	 *            Number of read bytes
	 * @return Byte array containing the read bytes
	 * @throws IOException - if some error occurs
	 */
	byte[] read_file(int file, int offset, int length) throws IOException;

}
