package de.hsrm.cs.wwwvs.filesystem.messages;

import java.nio.ByteBuffer;

public class WriteFileRequest implements Payload {
	private int handle;
	private int offset;
	private byte[] data;

	public WriteFileRequest(int handle, int offset, byte[] data) {
		super();
		this.handle = handle;
		this.offset = offset;
		this.data = data;
	}

	public int getHandle() {
		return handle;
	}

	public int getOffset() {
		return offset;
	}

	public byte[] getData() {
		return data;
	}

	@Override
	public void unmarshall(ByteBuffer data) throws MarshallingException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public byte[] marshall() throws MarshallingException {
		// TODO Auto-generated method stub
		return null;
	}

}
