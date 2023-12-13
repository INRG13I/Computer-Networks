import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Date;

public class UdpServer {
    private final DatagramSocket datagramSocket;
    private byte[] buffer = new byte[300];

    public UdpServer(DatagramSocket datagramSocket) {
        this.datagramSocket = datagramSocket;
    }

    public void run() throws IOException {
        while (true) {
            receive_then_send();
        }
    }

    private void receive_then_send() throws IOException {
        DatagramPacket datagramPacket = new DatagramPacket(buffer, 0, buffer.length);
        datagramSocket.receive(datagramPacket);
        InetAddress inetAddress = datagramPacket.getAddress();
        int port = datagramPacket.getPort();
        String message_from_client = new String(datagramPacket.getData(), 0, datagramPacket.getLength());

        String[] dateTimeStrings=message_from_client.split("-");
            String responseMessage =dateTimeStrings[0]+"-"+dateTimeStrings[1]+"-"+dateTimeStrings[2];
            buffer = responseMessage.getBytes();

            // Send the response back to the client
            datagramPacket = new DatagramPacket(buffer, buffer.length, inetAddress, port);
            datagramSocket.send(datagramPacket);
        
    }

    public static void main(String[] args) throws SocketException {
        DatagramSocket socket = new DatagramSocket(2345);
        UdpServer server = new UdpServer(socket);
        try {
            server.run();
        } catch (IOException exception) {
            System.out.println(exception.getMessage());
        }
    }
}