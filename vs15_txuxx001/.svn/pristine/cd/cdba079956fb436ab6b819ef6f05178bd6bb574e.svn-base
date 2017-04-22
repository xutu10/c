package de.hsrm.cs.wwwvs.filesystem.messages;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;


public class FolderInfoResponse implements Payload {
	private int parent;
	private String name;
	private int[] files;
	private int[] folders;

	public FolderInfoResponse() {
		
	}
	
	public FolderInfoResponse(int parent, String name, int[] files,
			int[] folders) {
		super();
		this.parent = parent;
		this.name = name;
		this.files = files;
		this.folders = folders;
	}

	public int getParent() {
		return parent;
	}

	public String getName() {
		return name;
	}

	public int[] getFiles() {
		return files;
	}

	public int[] getFolders() {
		return folders;
	}

	@Override
	public void unmarshall(ByteBuffer data) throws MarshallingException {
	}

	@Override
	public byte[] marshall() throws MarshallingException {
		// TODO Auto-generated method stub
		return null;
	}

}
