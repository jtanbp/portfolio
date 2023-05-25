import java.util.HashMap;
import java.util.Map;

public class DNSCache {
    //Saved
    private final HashMap<DNSQuestion, DNSRecord> cache;

    public DNSCache() {
        this.cache = new HashMap<>();
    }

    public DNSRecord query(DNSQuestion question) {
        DNSRecord record = cache.get(question);

        if (record == null) {
            return null;
        }

        if (record.isExpired()) {
            cache.remove(question);
            return null;
        }
        return record;
    }

    public void insert(DNSQuestion question, DNSRecord record) {
        cache.put(question, record);
    }
}