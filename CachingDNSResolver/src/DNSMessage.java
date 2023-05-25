import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class DNSMessage {
    //Saved
    public DNSHeader header = new DNSHeader();
    protected static DNSQuestion questions[];
    protected static DNSRecord answers[];
    public static DNSRecord authorityRecords[];
    public static DNSRecord additionalRecords[];
    private byte[] data;

    public static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        // decode the bytes into a DNSMessage object and return it
        DNSMessage message = new DNSMessage();
        ByteArrayInputStream ba_inputstream = new ByteArrayInputStream(bytes);

        // Store the original byte array in the DNSMessage object
        message.data = bytes;

        // Read the header from the input stream and set it in the DNSMessage object
        message.header = DNSHeader.decodeHeader(ba_inputstream);

        //Setting Size of Array
        questions = new DNSQuestion[message.header.getQueryCount()];
        answers = new DNSRecord[message.header.getAnswerCount()];
        authorityRecords = new DNSRecord[message.header.getAuthorityCount()];
        additionalRecords = new DNSRecord[message.header.getAdditionalCount()];

        // Read the questions from the input stream
        for (int i = 0; i < questions.length; i++) {
            questions[i] = DNSQuestion.decodeQuestion(ba_inputstream, message);
        }

        // Read the answers from the input stream
        for (int i = 0; i < answers.length; i++) {
            answers[i] = DNSRecord.decodeRecord(ba_inputstream, message);
        }

        // Read the authority records from the input stream
        for (int i = 0; i < authorityRecords.length; i++) {
            authorityRecords[i] = DNSRecord.decodeRecord(ba_inputstream, message);
        }

        // Read the additional records from the input stream
        for (int i = 0; i < additionalRecords.length; i++) {
            additionalRecords[i] = DNSRecord.decodeRecord(ba_inputstream, message);
        }

        // Return the DNSMessage object
        return message;
    }

    public String[] readDomainName(ByteArrayInputStream ba_inputStream) throws IOException {
        // read the pieces of a domain name starting from the current position of the input stream

        ArrayList<String> domainNamePieces = new ArrayList<>();
        int segmentLength = ba_inputStream.read();

        while (segmentLength != 0 ) {
            byte[] segmentBytes = new byte[segmentLength];
            ba_inputStream.read(segmentBytes);
            domainNamePieces.add(new String(segmentBytes));
            segmentLength = ba_inputStream.read();
        }
        //System.out.println("domain array size: " + domainNamePieces.size());
        String[] domainArray = new String[domainNamePieces.size()];
        for (int i = 0; i < domainArray.length; i++){
            domainArray[i] = domainNamePieces.get(i);
        }
        //return domainNamePieces.toArray(new String[domainNamePieces.size()]);
        return domainArray;
    }

    public String[] readDomainName(int firstByte) throws IOException {
        // read the pieces of a domain name from the specified byte
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(data, firstByte, data.length - firstByte);
        return readDomainName(byteArrayInputStream);
    }

    public static DNSMessage buildResponse(DNSMessage request, DNSRecord[] answers) {
        DNSMessage response = new DNSMessage();

        // Copy the questions from the request
        response.questions = request.questions;

        // Set the answers
        response.answers = answers;

        // Set the authority and additional records to empty arrays
        response.authorityRecords = request.authorityRecords;
        response.additionalRecords = request.additionalRecords;

        // Build the header
        response.header = DNSHeader.buildHeaderForResponse(request, response);

        return response;
    }

    public byte[] toBytes() throws IOException {
        // get the bytes to put in a packet and send back
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        // Write the header to the output stream
        header.writeBytes(outputStream);

        // Create a HashMap to store the locations of each domain name in the message
        HashMap<String, Integer> domainLocations = new HashMap<>();

        // Write each question to the output stream
        for (DNSQuestion question : questions) {
            question.writeBytes(outputStream, domainLocations);
        }

        // Write each answer to the output stream
        for (DNSRecord answer : answers) {
            answer.writeBytes(outputStream, domainLocations);
        }

        // Write each authority to the output stream
        for (DNSRecord authorityRecord : authorityRecords) {
            authorityRecord.writeBytes(outputStream, domainLocations);
        }

        // Write each additional to the output stream
        for (DNSRecord additionalRecord : additionalRecords) {
            additionalRecord.writeBytes(outputStream, domainLocations);
        }

        // Return the final bytes of the DNS message
        return outputStream.toByteArray();
    }

    public static void writeDomainName(ByteArrayOutputStream ba_outputstream, HashMap<String, Integer> domainLocations, String[] domainPieces) throws IOException {
        // write a domain name in the DNS encoding (each segment of the domain prefixed with its length, 0 at the end)
        // add it to the hash map if it's the first time the domain name is seen
        // otherwise, write a back pointer to where the domain has been seen previously

        String domain = joinDomainName(domainPieces);

        if (domainLocations.containsKey(domain)) {
            // Write a back pointer to the location where the domain has been seen previously
            int position = domainLocations.get(domain);
            ba_outputstream.write((position >> 8) | 0xc0); // to indicate its a pointer to location
            ba_outputstream.write(position & 0xff);
        } else {
            // Write the domain using DNS encoding
            int position = ba_outputstream.size();
            for (String piece : domainPieces) {
                ba_outputstream.write(piece.length());
                ba_outputstream.write(piece.getBytes());
            }
            ba_outputstream.write(0);
            domainLocations.put(domain, position);
        }
    }

    public static String joinDomainName(String[] pieces) {
        // join the pieces of a domain name with dots ([ "utah", "edu"] -> "utah.edu" )
        return String.join(".", pieces);
    }

    public String toString() {
        // return a string representation of the DNSMessage object
        StringBuilder sb = new StringBuilder();

        sb.append("DNS Message:\n");
        sb.append(header.toString());
        sb.append("\nQuestions:\n");
        for (DNSQuestion question : questions) {
            sb.append(question.toString());
            sb.append("\n");
        }
        sb.append("\nAnswers:\n");
        for (DNSRecord answer : answers) {
            sb.append(answer.toString());
            sb.append("\n");
        }

        return sb.toString();
    }
}