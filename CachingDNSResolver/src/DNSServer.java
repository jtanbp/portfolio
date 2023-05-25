import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;

public class DNSServer {
    //Saved
    private static int port = 8053;
    private static DNSCache cache = new DNSCache();

    // Since method only accepts array, a helper function is made to process the List into an array.
    public static DNSRecord[] getAnswerArray(List<DNSRecord> answers) {
        DNSRecord answers_array[] = new DNSRecord[answers.size()];
        for (int i = 0; i < answers.size(); i++){
            answers_array[i] = answers.get(i);
        }
        return answers_array;
    }

    public static void main(String[] args) throws IOException {
        DatagramSocket serverSocket = new DatagramSocket(port);
        byte[] receiveData = new byte[1024];
        while (true) {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            serverSocket.receive(receivePacket);
            byte[] datarequest = receivePacket.getData();
            // Packet decoded into DNSMessage request
            DNSMessage request = DNSMessage.decodeMessage(datarequest);
            DNSMessage response;

            List<DNSRecord> answers = new ArrayList<>();

            for (DNSQuestion question : request.questions) {
                DNSRecord record = cache.query(question);
                if (record != null) {
                    answers.add(record);
                } else {
                    // repackage data to request from google
                    byte[] sendData = request.toBytes();
                    DatagramPacket sendPacket2 = new DatagramPacket(sendData, sendData.length, InetAddress.getByName("8.8.8.8"), 53);
                    serverSocket.send(sendPacket2);

                    // Receive packet from google
                    byte[] receiveDataFromGoogle = new byte[1024];
                    DatagramPacket receivePacketFromGoogle = new DatagramPacket(receiveDataFromGoogle, receiveDataFromGoogle.length);
                    serverSocket.receive(receivePacketFromGoogle);

                    // decode message from google into DNSMessage response
                    response = DNSMessage.decodeMessage(receivePacketFromGoogle.getData());

                    for (DNSRecord answer : response.answers) {
                        cache.insert(question, answer);
                        answers.add(answer);
                        break;
                    }
                }
            }

            // repackage data to be sent out.
            DNSMessage newresponse = DNSMessage.buildResponse(request, getAnswerArray(answers));
            byte[] sendData = newresponse.toBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, receivePacket.getAddress(), receivePacket.getPort());
            serverSocket.send(sendPacket);
        }
    }
}