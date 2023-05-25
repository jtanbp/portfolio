import java.io.*;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;

public class DNSRecord {
    //Saved
    public String[] domain;
    public short type;
    public short classType;
    public int timeToLive;
    public byte[] data;
    public Date creationDate;

     public static DNSRecord decodeRecord(ByteArrayInputStream ba_inputstream, DNSMessage message) throws IOException {
        DNSRecord record = new DNSRecord();

        // Mark location to reset to if first 2 bits are not a pointer
        ba_inputstream.mark(0);
        int firstByte = ba_inputstream.read();

        // Check if first two bits indicate a pointer
        if((firstByte & 0xC0) == 0xC0) {
            int secondByte = ba_inputstream.read();
            int pointer = ((firstByte & 0x3F) << 8 | secondByte);
            record.domain = message.readDomainName(pointer); // read in the domainName
        }
        else {
            ba_inputstream.reset();
            record.domain = message.readDomainName(ba_inputstream); //should point to nothing, hence it should write 0
        }

        // Type
        record.type = (short) ( ba_inputstream.read() << 8 | ba_inputstream.read());

        // ClassType
        record.classType = (short) (ba_inputstream.read() << 8 | ba_inputstream.read());

        // TTL
        record.timeToLive = ba_inputstream.read() << 24 | ba_inputstream.read() << 16 | ba_inputstream.read() << 8 | ba_inputstream.read();

        // RLength
        int dataLength = ba_inputstream.read() << 8 | ba_inputstream.read();

        // RData
        record.data = new byte[dataLength];
        ba_inputstream.read(record.data, 0, dataLength);

        // creationDate
        record.creationDate = new Date();
        return record;
    }

    public void writeBytes(ByteArrayOutputStream ba_outputstream, HashMap<String, Integer> domainNameLocations) throws IOException {
        DNSMessage.writeDomainName(ba_outputstream, domainNameLocations, domain);
        ba_outputstream.write((type >> 8) & 0xff);
        ba_outputstream.write(type & 0xff);
        ba_outputstream.write((classType >> 8) & 0xff);
        ba_outputstream.write(classType & 0xff);
        ba_outputstream.write((timeToLive >> 24) & 0xff);
        ba_outputstream.write((timeToLive >> 16) & 0xff);
        ba_outputstream.write((timeToLive >> 8) & 0xff);
        ba_outputstream.write(timeToLive & 0xff);
        ba_outputstream.write((data.length >> 8) & 0xff);
        ba_outputstream.write(data.length & 0xff);
        ba_outputstream.write(data, 0, data.length);
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();

        sb.append("domain name: ");
        for (String name: domain){
            sb.append(name + " ");
        }
        sb.append("\n");

        sb.append("Type: " + type);
        sb.append(", classType: " + classType);
        sb.append(", TTL: " + timeToLive);
        sb.append(", Rlength: " + data.length);
        sb.append(", Date: " + creationDate + "\n");
        return sb.toString();
    }

    public boolean isExpired() {
        Calendar cal = Calendar.getInstance();
        cal.setTime(creationDate);
        cal.add(Calendar.SECOND, timeToLive);
        return cal.getTime().before(new Date());
    }
}