import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Objects;

public class DNSQuestion {
    //Saved
    private String[] domainName;
    private short queryType;
    private short queryClass;
    private DNSMessage dnsMessage;

    public static DNSQuestion decodeQuestion(ByteArrayInputStream ba_inputstream, DNSMessage dnsMessage) throws IOException {
        DNSQuestion question = new DNSQuestion();
        question.dnsMessage = dnsMessage;
        question.domainName = dnsMessage.readDomainName(ba_inputstream);
        question.queryType = (short) ((ba_inputstream.read() << 8) | ba_inputstream.read());
        question.queryClass = (short) ((ba_inputstream.read() << 8) | ba_inputstream.read());
        return question;
    }

    public void writeBytes(ByteArrayOutputStream OutputStream, HashMap<String, Integer> domainNameLocations) throws IOException {
        DNSMessage.writeDomainName(OutputStream, domainNameLocations, domainName);
        OutputStream.write((queryType >> 8 ) & 0xFF);
        OutputStream.write(queryType & 0xFF);
        OutputStream.write((queryClass >> 8 ) & 0xFF);
        OutputStream.write(queryClass & 0xFF);
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(dnsMessage.joinDomainName(domainName) + "\t");
        switch(queryClass) {
            case(1):
                sb.append("IN");
                break;
            case(3):
                sb.append("CH");
                break;
            case(4):
                sb.append("HS");
                break;
            case(255):
                sb.append("ANY");
                break;
            default:
                sb.append("Not yet created");
        }
        sb.append("\t");

        switch(queryType) {
            case(1):
                sb.append("A");
                break;
            case(2):
                sb.append("B");
                break;
            case(3):
                sb.append("C");
                break;
            case(4):
                sb.append("D");
                break;
            case(5):
                sb.append("E");
                break;
            case(6):
                sb.append("F");
                break;
            case(7):
                sb.append("G");
                break;
            case(8):
                sb.append("H");
                break;
            case(9):
                sb.append("I");
                break;
            case(10):
                sb.append("J");
                break;
            default:
                sb.append("Not yet created");
        }
        sb.append("\t");

        return sb.toString();
    }

    @Override
    public boolean equals(Object o) {
//        if (this == o) return true;
//        if (o == null || getClass() != o.getClass()) return false;
//        DNSQuestion that = (DNSQuestion) o;
//        return queryType == that.queryType &&
//                queryClass == that.queryClass &&
//                Objects.equals(domainName, that.domainName);
        return this.hashCode() == o.hashCode();
    }

    @Override
    public int hashCode() {
        return Objects.hash(DNSMessage.joinDomainName(domainName), queryType, queryClass);
    }
}