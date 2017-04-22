package de.hsrm.cs.wwwvs.filesystem.messages.marshalling;

import java.nio.ByteBuffer;

public interface Marshaller {
	
	/**
	 * MarshallingException wird geworfen, wenn es Fehler beim 
	 * Marshalling/Unmarshalling gibt.
	 * @author k_beckma
	 *
	 */
	static class MarshallingException extends Exception {
		private static final long serialVersionUID = 1L;

		public MarshallingException(String error) {
			super(error);
		}

		public MarshallingException(String error, Throwable t) {
			super(error, t);
		}
	}
	
	/**
	 * Interface für das Unmarshalling. Die Daten aus dem übergebenen
	 * ByteBuffer werden dekodiert und die Informationen in dem implementierenden
	 * Objekt gespeichert. Enthalten sollte nur der Payload sein, ohne den Header.
	 * 
	 * Achtung: diese Methode ist für die Client-Seite nur von den *Response-Payload-Typen 
	 * zu implementieren. 
	 * 
	 * @param data - ByteBuffer mit den empfangenen serialisierten Daten.
	 * @throws MarshallingException
	 */
	public void unmarshall(ByteBuffer data) throws MarshallingException;
	
	/**
	 * Interface für das Marshalling. Die Daten des implementierenden Payload-Objekts sollen in 
	 * ein Byte-Array kodiert werden. 
	 * 
	 * Achtung: diese Methode ist für die Client-Seite nur von den *Request-Payload-Typen
	 * zu implementieren.
	 * 
	 * @return - Byte-Array mit den serialisierten Daten des implementierenden Objekts.
	 * @throws MarshallingException
	 */
	public byte[] marshall() throws MarshallingException;

}
