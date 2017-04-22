package de.hsrm.cs.wwwvs.filesystem.cli;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import de.hsrm.cs.wwwvs.filesystem.Filesystem;

/**
 * Einfacher CLI Client für FileServer-Aufgabe
 * 
 * @author Andreas Textor
 * @author k_beckma (Angepasst auf neues API)
 */
public class ClientCLI {
	private static Filesystem fs;
	private static int currentFolder;

	private interface ArgRunnable {
		void run(String arg) throws Throwable;
	}

	final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();

	public static String bytesToHex(byte[] bytes) {
		char[] hexChars = new char[bytes.length * 2];
		for (int j = 0; j < bytes.length; j++) {
			int v = bytes[j] & 0xFF;
			hexChars[j * 2] = hexArray[v >>> 4];
			hexChars[j * 2 + 1] = hexArray[v & 0x0F];
		}
		return new String(hexChars);
	}

	enum Command {
		ls("ls", "Listet Dateien und Verzeichnisse", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				final int c = ClientCLI.currentFolder;
				int items = fs.get_folder_file_count(c) + fs.get_folder_folder_count(c);
						
				System.out.printf("drw-r--r- %4d %s\n", items, ".");

				int parentDir = fs.get_folder_parent(c);
				if (parentDir >= 0) {
					items = fs.get_folder_file_count(parentDir) + fs.get_folder_folder_count(parentDir);
					System.out.printf("drw-r--r- %4d %s\n", items, "..");
				}

				for (int d : fs.get_folder_folders(c)) {
					items = fs.get_folder_file_count(d) + fs.get_folder_folder_count(d);
					System.out.printf("drw-r--r- %4d %s\n", items, fs.get_folder_name(d));
				}
				for (int f : fs.get_folder_files(c)) {
					System.out.printf(" rw-r--r- %4d %s\n", fs.get_file_size(f), fs.get_file_name(f));
				}
			}
		}), pwd("pwd", "Gibt das aktuelle Verzeichnis aus", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				System.out.println(getFullPath(ClientCLI.currentFolder));
			}
		}), mkdir("mkdir", "Legt ein neues Verzeichnis an", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				fs.new_folder(arg, ClientCLI.currentFolder);
			}
		}), touch("touch", "Legt eine neue Datei an", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				fs.new_file(arg,ClientCLI.currentFolder);
			}
		}), cd("cd", "Wechselt in ein Verzeichnis", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				if (arg == null) {
					currentFolder = fs.get_root_folder();
				} else if (arg.equals("..")) {
					int parentDir = fs.get_folder_parent(currentFolder);
					if (parentDir >= 0)
						currentFolder = parentDir;
				} else {
					final int d = getDirectory(currentFolder, arg);

					if (d < 0) {
						System.out.printf("Verzeichnis nicht gefunden: %s\n", arg);
					} else {
						currentFolder = d;
					}
				}
			}
		}), cat("cat", "Gibt den Inhalt einer Datei aus", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				final int f = getFile(currentFolder, arg);
				if (f < 0) {
					System.out.printf("Datei nicht gefunden: %s\n", arg);
				} else {
					byte[] b = fs.read_file(f, 0, fs.get_file_size(f));
					System.out.println(new String(b));
				}
			}
		}), cathex("cathex", "Gibt den Inhalt einer Datei in hex aus", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				final int f = getFile(currentFolder, arg);
				if (f < 0) {
					System.out.printf("Datei nicht gefunden: %s\n", arg);
				} else {
					byte[] b = fs.read_file(f, 0, fs.get_file_size(f));
					System.out.println(bytesToHex(b));
				}
			}
		}), edit("edit",
				"Editiert eine Datei an einem Offset. Nutzung: edit datei offset inhalt, z.B.: edit foo 0 Hallo Welt",
				new ArgRunnable() {
					public void run(String arg) throws Throwable {
						final String[] a = arg.split(" ", 3);
						final String filename = a[0];
						final int offset = Integer.parseInt(a[1]);
						final String content = a[2];
						final int f = getFile(currentFolder, filename);

						if (f < 0) {
							System.out.printf("Datei nicht gefunden: %s\n", filename);
						} else {
							fs.write_file(f, offset, content.getBytes());
						}
					}
				}), echo("echo", "Schreibt in eine Datei. Nutzung: echo datei inhalt, z.B.: echo foo Hallo Welt",
				new ArgRunnable() {
					public void run(String arg) throws Throwable {
						final String[] a = arg.split(" ", 2);
						if (a == null || a.length < 2) {
							System.out.println(Command.echo.helpString);
							return;
						}

						final String fileName = a[0];
						int f = getFile(currentFolder, fileName);
						if (f < 0) {
							f = fs.new_file(fileName, currentFolder);
						}
						fs.write_file(f, 0, a[1].getBytes());

					}
				}), rm("rm", "Löscht eine Datei", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				final int f = getFile(currentFolder, arg);
				if (f > 0) {
					fs.delete_file(f);
				}
			}
		}), rmdir("rmdir", "Löscht ein Verzeichnis", new ArgRunnable() {
			public void run(String arg) throws Throwable {
				final int d = getDirectory(currentFolder, arg);
				if (d > 0) {
					fs.delete_folder(d);
				}
			}
		}), help("help", "Gibt die möglichen Befehle aus", new ArgRunnable() {
			public void run(String arg) {
				for (Command c : Command.values()) {
					System.out.printf("%10s - %s\n", c, c.helpString);
				}
			}
		});

		private final String cmd;
		private final String helpString;
		private final ArgRunnable runner;

		Command(final String cmd, final String helpString, final ArgRunnable runner) {
			this.cmd = cmd;
			this.helpString = helpString;
			this.runner = runner;
		}

		void run(final String arg) {
			try {
				this.runner.run(arg);
			} catch (final Throwable t) {
				System.out.printf("Fehler beim Ausführen von Kommando %s: %s: %s\n", this,
						t.getClass().getSimpleName(), t.getMessage());
				t.printStackTrace();
			}
		}
	}

	public static int getDirectory(int parentDir, String name) throws IOException {
		for (int dir : fs.get_folder_folders(parentDir)) {
			if (fs.get_folder_name(dir).equals(name))
				return dir;
		}
		return -1;
	}

	public static int getFile(int parentDir, String name) throws IOException{
		for (int file : fs.get_folder_files(parentDir)) {
			if (fs.get_file_name(file).equals(name))
					
				return file;
		}
		return -1;
	}

	public static String getFullPath(int currentDir) throws IOException {
		String path = fs.get_folder_name(currentDir);
		int parentDir = fs.get_folder_parent(currentDir);
		while (parentDir > 0) {
			path = fs.get_folder_name(parentDir) + "/" + path;
			parentDir = fs.get_folder_parent(parentDir);
		}

		return path;
	}

	public ClientCLI(final Filesystem filesystem) {
		fs = filesystem;
		try {
			currentFolder = fs.get_root_folder();
		} catch (Throwable t) {
			System.out.println("Konnte Wurzelverzeichnis nicht lesen");
			t.printStackTrace();
			return;
		}

		final BufferedReader d = new BufferedReader(new InputStreamReader(System.in));

		for (;;) {
			try {

				System.out.printf("%s > ", getFullPath(currentFolder));
			} catch (Throwable t) {
				System.out.printf("<ERROR: %s> >", t.getMessage());
				t.printStackTrace();
			}

			String s;

			try {
				s = d.readLine();
			} catch (IOException e) {
				System.out.println("Fehler bei Eingabe");
				return;
			}

			Command cmd = null;

			final String[] inp = s.split(" ", 2);
			for (Command c : Command.values()) {
				if (c.cmd.equals(inp[0]))
					cmd = c;
			}

			if (cmd == null) {
				System.out.println("Ungültiger Befehl.");
				Command.help.run(null);
				continue;
			}

			String arg = inp.length > 1 ? inp[1] : null;

			try {
				cmd.run(arg);
			} catch (Throwable t) {
				System.out.printf("Fehler bei Aufruf von Befehl: %s: %s: %s\n", arg, t.getClass().getSimpleName(),
						t.getMessage());
				t.printStackTrace();
			}
		}
	}
}