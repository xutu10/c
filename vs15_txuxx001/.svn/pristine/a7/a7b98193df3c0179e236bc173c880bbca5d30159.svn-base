package de.hsrm.cs.wwwvs.filesystem.messages;

import java.nio.ByteBuffer;

import de.hsrm.cs.wwwvs.filesystem.messages.marshalling.Marshaller;

public class FileServerMessage implements Marshaller {
	private PayloadType payloadType;
	private Payload payload;

	
	public FileServerMessage() {
		super();
	}
	public FileServerMessage(PayloadType payloadType, Payload payload) {
		super();
		this.payloadType = payloadType;
		this.payload = payload;
	}

	public PayloadType getPayloadType() {
		return payloadType;
	}

	public Payload getPayload() {
		return payload;
	}

	@Override
	public void unmarshall(ByteBuffer data) throws MarshallingException {
		
	}

	@Override
	public byte[] marshall() throws MarshallingException {
	
		return null;
	}
}
